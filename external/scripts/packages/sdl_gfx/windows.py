#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2gfxWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build SDL2_gfx
        _check = self.env.install_lib_path / self.config.get("checker")
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
        self.env.patch_static_MSVC("SDL2_gfx.vcxproj", self.env.BUILD_TYPE)

        cmd = '''msbuild SDL2_gfx.sln \
                    /maxcpucount:{} \
                    /t:SDL2_gfx \
                    /p:PlatformToolSet={} \
                    /p:Configuration={} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # Copy headers
        self.tag_log("Copying header files ..")
        copy2(Path(f'{sdl2_gfx_path}\\SDL2_framerate.h'),
              Path(f'{self.env.install_include_path}'))
        copy2(Path(f'{sdl2_gfx_path}\\SDL2_gfxPrimitives.h'),
              Path(f'{self.env.install_include_path}'))
        copy2(Path(f'{sdl2_gfx_path}\\SDL2_gfxPrimitives_font.h'),
              Path(f'{self.env.install_include_path}'))
        copy2(Path(f'{sdl2_gfx_path}\\SDL2_imageFilter.h'),
              Path(f'{self.env.install_include_path}'))
        copy2(Path(f'{sdl2_gfx_path}\\SDL2_rotozoom.h'),
              Path(f'{self.env.install_include_path}'))

    def patch_sdl2_gfx(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        _include = Path(Path(self.env.install_path) / 'include')
        _library = Path(Path(self.env.install_path) / '$(Configuration)')
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
