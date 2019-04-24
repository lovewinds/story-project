#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2TTFWindowsBuilder(PlatformBuilder):
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

    def patch_sdl2_ttf(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        _include = PureWindowsPath(Path(self.env.output_path) / 'include')
        _library = PureWindowsPath(Path(self.env.output_path) / 'SDL2/$(Configuration)')
        list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in list:
            item = child.find(msvc_ns_prefix+"ClCompile")
            item = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
            # item.text = "..\\..\\..\\built\\include;"+item.text
            item.text = "{};{}".format(_include, item.text)

            item = child.find(msvc_ns_prefix+"Link")
            item = item.find(msvc_ns_prefix+"AdditionalLibraryDirectories")
            # item.text = "..\\..\\..\\built\\$(Configuration);"+item.text
            item.text = "{};{}".format(_library, item.text)

        self.tag_log("Patched")
        tree.write(path, encoding="utf-8", xml_declaration=True)

    def build(self):
        super().build()

        # Build SDL2_ttf
        # TODO: Selective debug/release output
        install_path = PureWindowsPath(f'{self.env.output_path}/release')

        _check = f'{install_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        sdl2_ttf_path = PureWindowsPath('{}\\{}\\VisualC'.format(
            self.env.source_path,
            self.config['name']))
        os.chdir(sdl2_ttf_path)
        self.patch_sdl2_ttf("SDL_ttf.vcxproj")
        self.patch_static_MSVC("SDL_ttf.vcxproj")
        cmd = '''msbuild SDL_ttf.sln \
                    /maxcpucount:{} \
                    /t:SDL2_ttf \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Release \
                    /p:Platform=x64 \
                    /p:OutDir={} \
                '''.format(self.env.NJOBS, self.env.compiler_version, install_path)
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # Copy headers
        self.tag_log("Copying header files ..")
        copy2(PureWindowsPath(f'{sdl2_ttf_path}\\..\\SDL_ttf.h'),
              PureWindowsPath(f'{self.env.output_include_path}\\SDL_ttf.h'))

        # Copy external libraries
        # TODO: Fix hardcoded arch
        copy2(f'{sdl2_ttf_path}\\x64\\Release\\libfreetype-6.dll', install_path)
        # copy2(f'{sdl2_ttf_path}\\x64\\Release\\zlib1.dll', install_path)
