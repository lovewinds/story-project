#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class protobufiOSBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/cmake/build'.format(
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
        # self.env.BUILD_FLAG,
        cmd = 'CMD_PREFIX={} {}/ios-build.sh protobuf'.format(
            self.env.install_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        self.create_framework_iOS()

    def create_framework_iOS(self):
        # Required path
        include_path = '{}/{}/src'.format(
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
        for folder, _, files in os.walk(include_path):
            for ff in files:
                if not ff.endswith('.proto') and not ff.endswith('.h'):
                    continue
                rel_path = os.path.relpath(Path(folder) / ff, include_path)
                dir_path, _ = os.path.split(rel_path)
                BuildEnv.mkdir_p(Path(_framework_header_dir) / dir_path)
                copy2(Path(folder) / ff, Path(_framework_header_dir) / rel_path)

        # Copy binaries
        self.tag_log("Framework : Copying binary  ...")
        BuildEnv.mkdir_p(_framework_dir)
        _lib_src_file = '{}/libprotobuf.a'.format(self.env.install_lib_path)
        _lib_dst_file = '{}/protobuf'.format(_framework_dir)
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
