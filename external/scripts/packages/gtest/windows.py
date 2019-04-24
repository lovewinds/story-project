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
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build gtest
        # TODO: Selective debug/release output
        install_path = PureWindowsPath(f'{self.env.output_path}/release')
        build_path = PureWindowsPath('{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        ))
        project_path = PureWindowsPath('{}/{}/build/googlemock/gtest'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = f'{install_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")

        patch_target = PureWindowsPath('{}/{}/googletest/include/gtest/internal/gtest-internal.h'.format(
            self.env.source_path,
            self.config['name']
        ))
        # self.patch_file_encoding(patch_target)


        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        ## TODO: Change toolset dynamically
        cmd = 'cmake .. -A x64'
        self.log(f'[CMD]:: {cmd}'.split())
        self.env.run_command(cmd, module_name=self.config['name'])

        os.chdir(project_path)
        # As CMake generates project file, so it can't be proceed in pre()
        self.patch_gtest("gtest.vcxproj")
        self.patch_gtest("gtest_main.vcxproj")

        cmd = '''msbuild gtest.sln \
                    /maxcpucount:{} \
                    /t:gtest;gtest_main \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Release \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS, self.env.compiler_version, install_path)
        self.log('\n    '.join(f'[CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

                # /p:NoWarn=4819 \
                # /p:WarningLevel=0 \
                # /clp:ErrorsOnly \

    def patch_gtest(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        patched = False
        for child in list:
            item = child.find(msvc_ns_prefix+"ClCompile")
            item = item.find(msvc_ns_prefix+"RuntimeLibrary")
            if item.text == 'MultiThreaded':
                item.text = "MultiThreadedDLL"
                patched = True
            elif item.text == 'MultiThreadedDebug':
                item.text = "MultiThreadedDebugDLL"
                patched = True

        if patched:
            self.tag_log("Patched")
        else:
            self.tag_log("It seems to be patched already")

        tree.write(path, encoding="utf-8", xml_declaration=True)

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
