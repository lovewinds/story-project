#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class gtestWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package)

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]

    def build(self):
        super().build()

        # Build gtest
        install_path = PureWindowsPath('{}/{}/release'.format(
            self.env.output_path,
            self.config['name']
        ))
        build_path = PureWindowsPath('{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = f'{install_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        print(build_path)
        os.chdir(build_path)

        os.system('cmake ..')
        self.patch_gtest("gtest.vcxproj")
        self.patch_gtest("gtest_main.vcxproj")

        # os.system('msbuild g3log.sln /t:g3logger;g3logger_shared /p:Configuration=Release /p:OutDir='+build_path_rel)
        os.system('''
            msbuild gtest.sln \
                /maxcpucount:{} \
                /t:gtest;gtest_main \
                /p:PlatformToolSet={} \
                /p:Configuration=Release \
                /p:Platform=x64 \
                /p:OutDir={}'''.format(
                    self.env.NJOBS, self.env.compiler_version, install_path))

    def patch_gtest(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in list:
            item = child.find(msvc_ns_prefix+"ClCompile")
            item = item.find(msvc_ns_prefix+"RuntimeLibrary")
            if item.text == 'MultiThreaded':
                item.text = "MultiThreadedDLL"
            elif item.text == 'MultiThreadedDebug':
                item.text = "MultiThreadedDebugDLL"

        self.tag_log("Patched")

        tree.write(path, encoding="utf-8", xml_declaration=True)
