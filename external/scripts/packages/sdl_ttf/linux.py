#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2TTFLinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        self._build_freetype()

    def build(self):
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )
        _check = f'{self.env.install_lib_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def _build_freetype(self):
        build_path = '{}/{}/external/freetype-2.4.12/build'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.install_bin_path+'/freetype-config'):
            self.log("[FreeType2] already built.")
            return

        self.log("[FreeType] Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name='FreeType')
