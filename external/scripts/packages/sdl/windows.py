#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2WindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        # super(PlatformBuilder, self, config_package, config_platform)
        super(SDL2WindowsBuilder, self).__init__(config_package, config_platform)

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


    def build(self):
        super().build()

        # Build SDL2
        build_path = '{}/{}/release'.format(
            self.env.output_path,
            self.config['name']
        )

        _check = f'{build_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        os.chdir('{}/{}/VisualC'.format(
            self.env.source_path,
            self.config['name']))
        self.patch_static_MSVC("SDL/SDL.vcxproj")
        self.patch_static_MSVC("SDLmain/SDLmain.vcxproj")
        # os.system('msbuild SDL.sln /t:SDL2;SDL2main /p:PlatformToolSet=v140 /p:Configuration=Debug /p:OutDir='+build_path_dbg)
        os.system('''
msbuild SDL.sln \
    /maxcpucount:3 \
    /t:SDL2;SDL2main \
    /p:PlatformToolSet={} \
    /p:Configuration=Release \
    /p:Platform=x64 \
    /p:OutDir={}'''
.format(self.env.compiler_version, build_path))

        # Copy headers
        self.tag_log("Copying header files ..")
        sdl2_include_path = '{}/{}/include'.format(
            self.env.source_path,
            self.config['name']
        )
        # include_path = '{}/{}/include'.format(
        #     self.env.output_path,
        #     self.config['name']
        # )
        # os.makedirs(sdl2_include_path, exist_ok=True)

        try:
            copytree(sdl2_include_path, self.env.output_include_path)
        except FileExistsError:
            self.tag_log('Header files are already exists. Ignoring.')
