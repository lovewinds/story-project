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

        # TODO: Change toolset dynamically
        cmd = '''cmake ..  \
                    -A x64 \
                    -DCMAKE_BUILD_TYPE={} \
                    -DCMAKE_INSTALL_PREFIX={} \
                    -DPYBIND11_LTO_CXX_FLAGS="" \
                    -DPYTHON_LIBRARY={}/python37.lib \
                    -DPYTHON_INCLUDE_DIR={}/python \
                    -DPYBIND11_INSTALL=ON \
                    -DPYBIND11_TEST=OFF \
                '''.format(self.env.BUILD_TYPE,
                           self.env.install_path,
                           self.env.install_lib_path,
                           self.env.install_include_path)
        self.log(f'     [CMD]:: {cmd}')
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '''cmake --build . \
                    --config {} \
                    --target INSTALL \
                '''.format(self.env.BUILD_TYPE)
        self.log(f'     [CMD]:: {cmd}')
        self.env.run_command(cmd, module_name=self.config['name'])
