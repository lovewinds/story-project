#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path, PureWindowsPath
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class boostWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package)

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]

    def build(self):
        pkg_path = PureWindowsPath('{}\\{}'.format(
            self.env.source_path,
            self.config['name']
        ))
        build_path = PureWindowsPath('{}\\{}\\build'.format(
            self.env.source_path,
            self.config['name']
        ))

        # if os.path.exists(self.env.output_lib_path+'\\libboost_python3.lib'):
        _check = f'{self.env.output_lib_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(pkg_path)
        # self.env.BUILD_FLAG,
        cmd = '''.\\bootstrap.bat \
             --with-python-root={} \
             --with-libraries=python \
             --prefix={}'''.format(
            self.env.output_path,
            self.env.output_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '''.\\b2 \
            -j {} \
            --build-dir=build\\x64 \
            --prefix={} \
            --with-python \
            --toolset={} \
            link=static \
            address-model=64 \
            threading=multi \
            install'''.format(
            self.env.output_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
