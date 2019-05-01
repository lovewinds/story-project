#!/usr/bin/env python3
import os
from shutil import copytree, copy2, move
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class protobufWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build protobuf
        build_path = Path('{}/{}/cmake/build'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = f'{self.env.install_lib_path}\\{self.config.get("checker")}'
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
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '''msbuild protobuf.sln \
                    /maxcpucount:{} \
                    /t:libprotobuf \
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
            move(f'{self.env.install_lib_path}\\libprotobufd.lib',
                 f'{self.env.install_lib_path}\\libprotobuf.lib')
