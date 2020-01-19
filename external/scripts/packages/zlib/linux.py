#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class zlibLinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        pkg_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        # Apply patch
        os.chdir(pkg_path)
        cmd = "python {}/patch.py {}/zlib-1.2.11.patch".format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} cmake -DCMAKE_INSTALL_PREFIX={} .. ; make -j{}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        # Install 'zlib.pc' file manually
        copy2(Path(build_path) / 'zlib.pc',
              self.env.install_lib_path / 'pkgconfig')
