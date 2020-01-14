#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2ImageiOSBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/Xcode-iOS'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        print("       [{}] Start building ...".format(self.config['name']))
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} CMD_PREFIX={} {}/ios-build.sh SDL2_image'.format(
            self.env.BUILD_FLAG,
            self.env.install_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()
        # Copy header file
        _header_source = '{}/{}/SDL_image.h'.format(
            self.env.source_path,
            self.config['name']
        )
        _header_dest = '{}/SDL_image.h'.format(
            self.env.install_include_path
        )
        self.tag_log("copying header ...")
        copy2(_header_source, _header_dest)

        self.create_framework_iOS()

    def create_framework_iOS(self):
        # Required path
        _include_path = '{}/{}'.format(
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
        _header_src_file = '{}/SDL_image.h'.format(_include_path)
        _header_dst_file = '{}/SDL_image.h'.format(_framework_header_dir)
        copy2(_header_src_file, _header_dst_file)

        # Copy binaries
        self.tag_log("Framework : Copying binary  ...")
        BuildEnv.mkdir_p(_framework_dir)
        _lib_src_file = '{}/libSDL2_image.a'.format(self.env.install_lib_path)
        _lib_dst_file = '{}/SDL2_image'.format(_framework_dir)
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
