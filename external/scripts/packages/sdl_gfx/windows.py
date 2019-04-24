#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2gfxWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def patch_static_MSVC(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        # Change build result to .lib
        list = root.findall(msvc_ns_prefix+"PropertyGroup")
        for child in list:
            try:
                item = child.find(msvc_ns_prefix+"ConfigurationType")
                if item.text == "DynamicLibrary":
                    item.text = "StaticLibrary"
            except:
                pass

        # Change runtime library
        list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in list:
            try:
                item = child.find(msvc_ns_prefix+"ClCompile")
                item = item.find(msvc_ns_prefix+"RuntimeLibrary")
                if item.text == 'MultiThreadedDLL':
                    item.text = "MultiThreaded"
                elif item.text == 'MultiThreadedDebugDLL':
                    item.text = "MultiThreadedDebug"
            except:
                pass

        self.tag_log("Patched")
        tree.write(path, encoding="utf-8", xml_declaration=True)

    def patch_sdl2_gfx(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        _include = PureWindowsPath(Path(self.env.output_path) / 'include')
        _library = PureWindowsPath(Path(self.env.output_path) / '$(Configuration)')
        list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in list:
            item = child.find(msvc_ns_prefix+"ClCompile")
            item = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
            item.text = "{};{}".format(_include, item.text)

            item = child.find(msvc_ns_prefix+"Link")
            item = item.find(msvc_ns_prefix+"AdditionalLibraryDirectories")
            item.text = "{};{}".format(_library, item.text)

        list = root.findall(msvc_ns_prefix+"PropertyGroup")
        for child in list:
            item = child.find(msvc_ns_prefix+"WindowsTargetPlatformVersion")
            if item != None:
                child.remove(item)

        self.tag_log("Patched")
        tree.write(path, encoding="utf-8", xml_declaration=True)

    def build(self):
        super().build()

        # Build SDL2_gfx
        # TODO: Selective debug/release output
        install_path = PureWindowsPath(f'{self.env.output_path}/release')

        _check = f'{install_path}\\{self.config.get("checker")}'
        self.tag_log('checking {}'.format(_check))
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        sdl2_gfx_path = '{}\\{}'.format(
            self.env.source_path,
            self.config['name'])

        # !REQUIRED! Patch additional library and include path
        os.chdir(sdl2_gfx_path)
        ## SDL2_gfx x64 patch
        # Use patch script
        # https://github.com/techtonik/python-patch
        self.tag_log('Patch for x64')
        cmd = "python {}/patch.py {}/sdl2_gfx.x64_solution.patch".format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
        cmd = "python {}/patch.py {}/sdl2_gfx.x64_project.patch".format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        self.patch_sdl2_gfx("SDL2_gfx.vcxproj")
        self.patch_static_MSVC("SDL2_gfx.vcxproj")

        cmd = '''msbuild SDL2_gfx.sln \
                    /maxcpucount:{} \
                    /t:SDL2_gfx \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Release \
                    /p:Platform=x64 \
                    /p:OutDir={} \
                '''.format(self.env.NJOBS, self.env.compiler_version, install_path)
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # Copy headers
        self.tag_log("Copying header files ..")
        copy2(PureWindowsPath(f'{sdl2_gfx_path}\\SDL2_framerate.h'),
              PureWindowsPath(f'{self.env.output_include_path}'))
        copy2(PureWindowsPath(f'{sdl2_gfx_path}\\SDL2_gfxPrimitives.h'),
              PureWindowsPath(f'{self.env.output_include_path}'))
        copy2(PureWindowsPath(f'{sdl2_gfx_path}\\SDL2_gfxPrimitives_font.h'),
              PureWindowsPath(f'{self.env.output_include_path}'))
        copy2(PureWindowsPath(f'{sdl2_gfx_path}\\SDL2_imageFilter.h'),
              PureWindowsPath(f'{self.env.output_include_path}'))
        copy2(PureWindowsPath(f'{sdl2_gfx_path}\\SDL2_rotozoom.h'),
              PureWindowsPath(f'{self.env.output_include_path}'))
