#!/usr/bin/env python3
import os
import tarfile
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class boostiOSBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        self._patch_boost_apple()

    def build(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        # if os.path.exists(self.env.framework_path+'/boost.framework'):
        _check = f'{self.env.framework_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        os.system('chmod +x boost.sh')
        # /Users/ariens/source/story-project/external/built
        cmd = '''{} ./boost.sh \
                -ios \
                --boost-version 1.64.0 \
                --boost-libs python \
                --python-root {} \
                --min-ios-version 7.0'''.format(
            self.env.BUILD_FLAG,
            self.env.install_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post_build_iOS(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )

        # Copy built framework
        if not os.path.exists(self.env.framework_path+'/boost.framework'):
            copytree(
                '{}/build/boost/1.64.0/ios/framework/boost.framework'.format(build_path),
                '{}/boost.framework'.format(self.env.framework_path)
            )

    def _patch_boost_apple(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        os.chdir(build_path)
        # cmd = 'python {}/patch.py {}/boost-use-python3.patch'.format(
        # 	self.env.working_path,
        # 	self.env.patch_path
        # )
        cmd = 'patch -t -p1 boost.sh < {}/boost-use-python3.patch'.format(
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
        self.tag_log("Patched")
