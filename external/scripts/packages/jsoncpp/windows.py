#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class jsoncppWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        src_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )

        self.tag_log("Start building ...")
        if os.path.exists(src_path+'/dist'):
            self.tag_log("Already built.")
            return

        # Generate amalgamated source and header for jsoncpp
        self.tag_log('Copying header files ...')
        os.chdir(src_path)
        cmd = 'python amalgamate.py'
        self.env.run_command(cmd, module_name=self.config['name'])
