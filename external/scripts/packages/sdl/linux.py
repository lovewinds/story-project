#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2LinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super(SDL2LinuxBuilder, self).__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = f'{self.env.output_lib_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            print("       [{}] already built.".format(self.config['name']))
            return

        print("       [{}] Start building ...".format(self.config['name']))
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.output_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])
