#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class protobufWindowsBuilder(PlatformBuilder):
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

    def build(self):
        super().build()

        # Build protobuf
        # TODO: Selective debug/release output
        install_path = PureWindowsPath(f'{self.env.output_path}/release')
        build_path = PureWindowsPath('{}/{}/cmake/build'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = f'{install_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        
        ## TODO: Change toolset dynamically
        cmd = '''cmake \
                    -Dprotobuf_BUILD_TESTS=OFF \
                    -A x64 \
                    ..'''
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '''msbuild protobuf.sln \
                    /maxcpucount:{} \
                    /t:libprotobuf \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Release \
                    /p:Platform=x64 \
                    /p:OutDir={}\
                '''.format(self.env.NJOBS, self.env.compiler_version, install_path)
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # required only debug release
        # os.rename(f'{install_path}\\libprotobufd.lib',
        #           f'{install_path}\\libprotobuf.lib')
