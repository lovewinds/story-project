#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class gtestiOSBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} CMD_PREFIX={} {}/ios-build.sh gtest armv7'.format(
            self.env.BUILD_FLAG,
            self.env.install_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '{} CMD_PREFIX={} {}/ios-build.sh gtest arm64'.format(
            self.env.BUILD_FLAG,
            self.env.install_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        # Copy header files also
        # _path = self.env.source_path / self.config['name'] / 'googletest/include/gtest'
        # copytree(_path, self.env.install_include_path)
        # _path = self.env.source_path / self.config['name'] / 'googlemock/include/gmock'
        # copytree(_path, self.env.install_include_path)

        self.create_framework_iOS()

    def create_framework_iOS(self):
        # Required path
        include_path = '{}/{}/include'.format(
            self.env.source_path,
            self.config['name']
        )
        _framework_dir = '{}/{}.framework'.format(
            self.env.framework_path,
            self.config['name'],
        )
        _framework_header_dir = '{}/{}.framework/Headers'.format(
            self.env.framework_path,
            self.config['name'],
        )
        _framework_resource_dir = '{}/{}.framework/Resources'.format(
            self.env.framework_path,
            self.config['name'],
        )

        # Copy headers into Framework directory
        self.tag_log("Framework : Copying header ...")
        BuildEnv.mkdir_p(_framework_header_dir)
        # _path = Path(include_path)
        if not os.path.isdir(Path(_framework_header_dir) / 'gtest'):
            _path = self.env.source_path / self.config['name'] / 'googletest/include/gtest'
            copytree(_path, f'{_framework_header_dir}/gtest')
        if not os.path.isdir(Path(_framework_header_dir) / 'gmock'):
            _path = self.env.source_path / self.config['name'] / 'googlemock/include/gmock'
            copytree(_path, f'{_framework_header_dir}/gmock')

        # Copy binaries
        self.tag_log("Framework : Copying binary  ...")
        BuildEnv.mkdir_p(_framework_dir)
        _lib_src_file = '{}/libgtest.a'.format(self.env.install_lib_path)
        _lib_dst_file = '{}/gtest'.format(_framework_dir)
        copy2(_lib_src_file, _lib_dst_file)

        # Create plist
        BuildEnv.mkdir_p(_framework_resource_dir)
        plist_str = self.env.apple_framework_plist.replace(
            '${FRAMEWORK_NAME}', self.config['name']
        ).replace(
            '${FRAMEWORK_CURRENT_VERSION}', self.config['name']
        )
        plist_file = '{}/Info.plist'.format(_framework_resource_dir)
        with open(plist_file, "w") as pf:
            pf.write(plist_str)
