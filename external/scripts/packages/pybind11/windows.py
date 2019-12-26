#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pybind11WindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build pybind11
        build_path = Path('{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        
        ## TODO: Change toolset dynamically
        cmd = '''cmake ..  \
                    -A x64 \
                    -DPYBIND11_LTO_CXX_FLAGS="" \
                    -DPYTHON_LIBRARY={}/python36.lib \
                    -DPYTHON_INCLUDE_DIR={}/python \
                '''.format(self.env.install_lib_path,
                           self.env.install_include_path)
        self.log(f'     [CMD]:: {cmd}')
        self.env.run_command(cmd, module_name=self.config['name'])

        # cmd = '''msbuild pybind11.sln \
        #             /maxcpucount:{} \
        #             /t:check \
        #             /p:PlatformToolSet={} \
        #             /p:Configuration={} \
        #             /p:Platform=x64 \
        #             /p:OutDir={}\\ \
        #         '''.format(self.env.NJOBS,
        #                    self.env.compiler_version, self.env.BUILD_TYPE,
        #                    self.env.install_lib_path)
        # self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        # self.env.run_command(cmd, module_name=self.config['name'])
