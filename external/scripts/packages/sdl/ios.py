#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2iOSBuilder(PlatformBuilder):
    def __init__(self, 
                 config_package: dict=None,
                 config_platform: dict=None):
        # super(PlatformBuilder, self, config_package, config_platform)
        super(SDL2iOSBuilder, self).__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/Xcode-iOS/SDL'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.output_lib_path+'/libSDL2.a'):
            print("       [{}] already built.".format(self.config['name']))
            return

        print("       [{}] Start building ...".format(self.config['name']))
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PREFIX={} {}/ios-build.sh SDL2'.format(
            self.env.BUILD_FLAG,
            self.env.output_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        # Copy header files also
        sdl2_include_path = '{}/{}/include'.format(
            self.env.source_path,
            self.config['name']
        )
        _path = Path(sdl2_include_path)
        _files = [x for x in _path.iterdir() if x.is_file()]
        for file in _files:
            copy2(str(file), self.env.output_include_path)

        # Overwrite to use iOS header
        header_ios_path = '{}/{}/include/SDL_config_iphoneos.h'.format(
            self.env.source_path,
            self.config['name']
        )
        sdl_config_header = '{}/SDL_config.h'.format(
            self.env.output_include_path
        )
        copy2(header_ios_path, sdl_config_header)
        print("       [{}] header file for iOS has been changed ...".format(self.config['name']))

        self.create_framework_iOS()

    def create_framework_iOS(self):
        # Required path
        sdl2_include_path = '{}/{}/include'.format(
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
        print("       [{}] Framework : Copying header ...".format(self.config['name']))
        BuildEnv.mkdir_p(_framework_header_dir)
        _path = Path(sdl2_include_path)
        _files = [x for x in _path.iterdir() if x.is_file()]
        for file in _files:
            copy2(str(file), _framework_header_dir)

        # Copy binaries
        print("       [{}] Framework : Copying binary  ...".format(self.config['name']))
        BuildEnv.mkdir_p(_framework_dir)
        _lib_src_file = '{}/libSDL2.a'.format(self.env.output_lib_path)
        _lib_dst_file = '{}/SDL'.format(_framework_dir)
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
