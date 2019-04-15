#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2gfxiOSBuilder(PlatformBuilder):
    def __init__(self, 
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package)

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]

    def build(self):
        build_parent_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )
        _check = f'{self.env.output_lib_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_parent_path)
        cmd = '{} PREFIX={} {}/ios-build.sh SDL2_gfx'.format(
            self.env.BUILD_FLAG,
            self.env.output_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()

        # Copy header file
        headers = [
            'SDL2_framerate.h',
            'SDL2_rotozoom.h',
            'SDL2_imageFilter.h',
            'SDL2_gfxPrimitives.h',
            'SDL2_gfxPrimitives_font.h'
        ]

        for header in headers:
            _header_source = '{}/{}/{}'.format(
                self.env.source_path,
                self.config['name'],
                header
            )
            _header_dest = '{}/{}'.format(
                self.env.output_include_path,
                header
            )
            self.tag_log("Copying header ...")
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
        headers = [
            'SDL2_framerate.h',
            'SDL2_rotozoom.h',
            'SDL2_imageFilter.h',
            'SDL2_gfxPrimitives.h',
            'SDL2_gfxPrimitives_font.h'
        ]

        for header in headers:
            _header_src_file = '{}/{}'.format(_include_path, header)
            _header_dst_file = '{}/{}'.format(_framework_header_dir, header)
            copy2(_header_src_file, _header_dst_file)

        # Copy binaries
        self.tag_log("Framework : Copying binary  ...")
        BuildEnv.mkdir_p(_framework_dir)
        _lib_src_file = '{}/libSDL2_gfx.a'.format(self.env.output_lib_path)
        _lib_dst_file = '{}/SDL_gfx'.format(_framework_dir)
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
