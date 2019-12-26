#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class boostLinuxBuilder(PlatformBuilder):
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

        # if os.path.exists(self.env.install_lib_path+'/libboost_python3.a'):
        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(pkg_path)
        # self.env.BUILD_FLAG,
        cmd = '''{} PYTHON=python3; PATH={}:$PATH; ./bootstrap.sh \
             --with-python-root={} \
             --with-libraries=python \
             --prefix={}'''.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.install_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '''{} ./b2 \
            --build-dir=build \
            --prefix={} \
            --with-python \
            link=static \
            cxxflags="-fPIC" \
            linkflags="-fPIC" install'''.format(
            self.env.BUILD_FLAG,
            self.env.install_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
