#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2ImageWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package)

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]


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

    def _patch(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        _include = PureWindowsPath(Path(self.env.output_path) / 'include')
        _library = PureWindowsPath(Path(self.env.output_path) / 'SDL2/$(Configuration)')
        list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in list:
            item = child.find(msvc_ns_prefix+"ClCompile")
            item_include = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
            item_include.text = "{};{}".format(_include, item_include.text)

            item = child.find(msvc_ns_prefix+"Link")
            item_lib = item.find(msvc_ns_prefix+"AdditionalLibraryDirectories")
            if item_lib == None:
                note = ElementTree.Element(msvc_ns_prefix+"AdditionalLibraryDirectories")
                # note.text = "..\\..\\..\\built\\$(Configuration);"
                note.text = f'{_library};'
                item.append(note)

        list = root.findall(msvc_ns_prefix+"ItemGroup")
        # print(list)
        for child in list:
            libPaths = child.findall(msvc_ns_prefix+"Library")
            _library = PureWindowsPath(Path(self.env.output_path) / 'SDL2/$(Configuration)')
            if libPaths == None:
                continue
            for dir in libPaths:
                dir.attrib["Include"] = dir.attrib["Include"].replace("..\\..\\SDL\\VisualC\\SDL\\$(Platform)\\$(Configuration)\\SDL2.lib",
                                                                      f'{_library}\\SDL2.lib')
                dir.attrib["Include"] = dir.attrib["Include"].replace("..\\..\\SDL\\VisualC\\SDLmain\\$(Platform)\\$(Configuration)\\SDL2main.lib",
                                                                      f'{_library}\\SDL2main.lib')

        tree.write(path, encoding="utf-8", xml_declaration=True)
        self.tag_log("Patched")

    def build(self):
        super().build()

        # Build SDL2_image
        # TODO: Selective debug/release output
        install_path = PureWindowsPath(f'{self.env.output_path}/release')

        _check = f'{install_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        sdl2_image_path = '{}\\{}\\VisualC'.format(
            self.env.source_path,
            self.config['name'])
        os.chdir(sdl2_image_path)
        # !REQUIRED! Patch additional library and include path
        #patch_sdl2_image("SDL_image_VS2012.vcxproj")
        self._patch("SDL_image.vcxproj")
        self.patch_static_MSVC("SDL_image.vcxproj")
        cmd = '''msbuild SDL_image.sln \
                    /maxcpucount:{} \
                    /t:SDL2_image \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Release \
                    /p:Platform=x64 \
                    /p:OutDir={} \
                '''.format(self.env.NJOBS, self.env.compiler_version, install_path)
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        ## Install
        self.tag_log("Copying header files ..")
        copy2(PureWindowsPath(f'{sdl2_image_path}\\..\\SDL_image.h'),
              PureWindowsPath(f'{self.env.output_include_path}\\SDL_image.h'))

        # Copy external libraries
        # TODO: Fix hardcoded arch
        copy2(f'{sdl2_image_path}\\external\\lib\\x64\\libjpeg-9.dll', install_path)
        copy2(f'{sdl2_image_path}\\external\\lib\\x64\\libpng16-16.dll', install_path)
        copy2(f'{sdl2_image_path}\\external\\lib\\x64\\libtiff-5.dll', install_path)
        copy2(f'{sdl2_image_path}\\external\\lib\\x64\\libwebp-4.dll', install_path)
        copy2(f'{sdl2_image_path}\\external\\lib\\x64\\zlib1.dll', install_path)
