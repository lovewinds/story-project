#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2TTFWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build SDL2_ttf
        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        sdl2_ttf_path = Path('{}\\{}\\VisualC'.format(
            self.env.source_path,
            self.config['name']))
        os.chdir(sdl2_ttf_path)
        self.patch_sdl2_ttf("SDL_ttf.vcxproj")
        self.env.patch_static_MSVC("SDL_ttf.vcxproj", self.env.BUILD_TYPE)
        cmd = '''msbuild SDL_ttf.sln \
                    /maxcpucount:{} \
                    /t:SDL2_ttf \
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
        copy2(Path(f'{sdl2_ttf_path}\\..\\SDL_ttf.h'),
              Path(f'{self.env.install_include_path}\\SDL_ttf.h'))

        # Copy external libraries
        self.tag_log("Copying external libraries ..")
        # TODO: Fix hardcoded arch
        copy2(f'{sdl2_ttf_path}\\external\\lib\\x64\\libfreetype-6.dll', self.env.install_lib_path)
        copy2(f'{sdl2_ttf_path}\\external\\lib\\x64\\libfreetype-6.lib', self.env.install_lib_path)
        # copy2(f'{sdl2_ttf_path}\\external\\lib\\x64\\zlib1.dll', self.env.install_lib_path)

    def patch_sdl2_ttf(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        _include = Path(self.env.install_include_path)
        _library = Path(self.env.install_lib_path)
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
