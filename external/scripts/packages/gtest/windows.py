#!/usr/bin/env python3
import os
from shutil import copytree, copy2, move
from pathlib import Path
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
        build_path = Path('{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        ))
        project_path = Path('{}/{}/build/googlemock/gtest'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = f'{self.env.install_lib_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")

        # patch_target = Path('{}/{}/googletest/include/gtest/internal/gtest-internal.h'.format(
        #     self.env.source_path,
        #     self.config['name']
        # ))
        # self.patch_file_encoding(patch_target)


        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        ## TODO: Change toolset dynamically
        cmd = 'cmake .. -A x64'
        self.log(f'[CMD]:: {cmd}'.split())
        self.env.run_command(cmd, module_name=self.config['name'])

        os.chdir(project_path)
        cmd = '''msbuild gtest.sln \
                    /maxcpucount:{} \
                    /t:gtest;gtest_main \
                    /p:PlatformToolSet={} \
                    /p:Configuration={} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # required only debug release
        if self.env.BUILD_TYPE == 'Debug':
            self.tag_log("Renaming built libraries ..")
            move(f'{self.env.install_lib_path}\\gtestd.lib',
                 f'{self.env.install_lib_path}\\gtest.lib')
            move(f'{self.env.install_lib_path}\\gtest_maind.lib',
                 f'{self.env.install_lib_path}\\gtest_main.lib')

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
