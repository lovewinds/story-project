#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class g3logWindowsBuilder(PlatformBuilder):
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

    def pre(self):
        super().pre()
        self.patch_g3log_remove_warnings()

    def build(self):
        super().build()

        # Build SDL2
        install_path = '{}/{}/release'.format(
            self.env.output_path,
            self.config['name']
        )
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = f'{install_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        # Build g3log
        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        # patch_g3log_remove_warnings(build_path)
        # TODO: Check 'CMAKE_BUILD_TYPE' is required if it builds both build type?
        #os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE='+BUILD_CONF+' -G "Visual Studio 12" ..')
        # Disable fatal signal handling on g3log 1.3
        os.system('''cmake \
            -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON \
            -DENABLE_FATAL_SIGNALHANDLING=OFF \
            -DADD_BUILD_WIN_SHARED=ON ..''')
        self.patch_static_MSVC("g3logger.vcxproj")

        # os.system('msbuild g3log.sln /t:g3logger;g3logger_shared /p:Configuration=Release /p:OutDir='+build_path_rel)
        os.system('''
            msbuild g3log.sln \
                /maxcpucount:{} \
                /t:g3logger;g3logger_shared \
                /p:PlatformToolSet={} \
                /p:Configuration=Release \
                /p:Platform=x64 \
                /p:OutDir={}'''.format(
                    self.env.NJOBS, self.env.compiler_version, install_path))

    def patch_g3log_remove_warnings(self):
        patch_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        os.chdir(patch_path)

        # Use patch script
        # https://github.com/techtonik/python-patch
        cmd = "python {}/patch.py {}/g3log-1.2-remove_warnings.patch".format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        self.tag_log("Patched")
