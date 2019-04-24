#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class g3logWindowsBuilder(PlatformBuilder):
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

    def pre(self):
        super().pre()
        self.patch_g3log_remove_warnings()

    def build(self):
        super().build()

        # Build g3log
        # TODO: Selective debug/release output
        install_path = PureWindowsPath(f'{self.env.output_path}/release')
        build_path = PureWindowsPath('{}/{}/build'.format(
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
        # patch_g3log_remove_warnings(build_path)
        # TODO: Check 'CMAKE_BUILD_TYPE' is required if it builds both build type?
        #os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE='+BUILD_CONF+' -G "Visual Studio 12" ..')
        # Disable fatal signal handling on g3log 1.3
        cmd = '''cmake \
            -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON \
            -DENABLE_FATAL_SIGNALHANDLING=OFF \
            -DADD_BUILD_WIN_SHARED=ON \
            -A x64 \
            ..'''
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # patch
        self.patch_static_MSVC("g3logger.vcxproj")

        # os.system('msbuild g3log.sln /t:g3logger;g3logger_shared /p:Configuration=Release /p:OutDir='+build_path_rel)
        cmd = '''msbuild g3log.sln \
                    /maxcpucount:{} \
                    /t:g3logger;g3logger_shared \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Release \
                    /p:Platform=x64 \
                    /p:OutDir={} \
                '''.format(self.env.NJOBS, self.env.compiler_version, install_path)
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()
        
        patch_target = PureWindowsPath('{}/{}/src/g3log/g3log.hpp'.format(
            self.env.source_path,
            self.config['name']
        ))
        self.patch_file_encoding(patch_target)


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

    def patch_file_encoding(self, path):
        import codecs
        BLOCKSIZE = 1048576 # or some other, desired size in bytes

        from shutil import copyfile
        backup = f'{path}.old'
        if not os.path.exists(backup):
            copyfile(path, backup)

        with codecs.open(backup, "r", "utf-8") as sourceFile:
            with codecs.open(path, "w", "utf-16") as targetFile:
                while True:
                    contents = sourceFile.read(BLOCKSIZE)
                    if not contents:
                        break
                    targetFile.write(contents)
