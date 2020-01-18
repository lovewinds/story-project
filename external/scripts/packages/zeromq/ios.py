#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class zeromqiOSBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        subpkg_url = 'https://github.com/zeromq/cppzmq/archive/v4.2.2.tar.gz'
        subpkg_name = 'cppzmq'
        subpkg_archive = 'cppzmq-4.2.2.tar.gz'

        self.tag_log("Preparing sub package")
        self.env.download_file(subpkg_url, subpkg_archive)
        self.env.extract_tarball(subpkg_archive, subpkg_name)

        # Patch
        self._patch_libzmq()

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
        cmd = 'CMD_PREFIX={} {}/ios-build.sh zeromq armv7'.format(
            self.env.install_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = 'CMD_PREFIX={} {}/ios-build.sh zeromq arm64'.format(
            self.env.install_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        # Copy header files also
        include_path = '{}/{}/src'.format(
            self.env.source_path,
            self.config['name']
        )
        _path = Path(include_path)
        _files = [x for x in _path.iterdir() if x.is_file()]
        for ff in _files:
            if not ff.name.endswith('.hpp') and not ff.name.endswith('.h'):
                continue
            copy2(str(ff), self.env.install_include_path)

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
        _path = Path(include_path)
        _files = [x for x in _path.iterdir() if x.is_file()]
        for ff in _files:
            if not ff.name.endswith('.hpp') and not ff.name.endswith('.h'):
                continue
            copy2(str(ff), _framework_header_dir)

        # Copy CPPZMQ Headers also
        _path = Path(self.env.source_path / 'cppzmq')
        _files = [x for x in _path.iterdir() if x.is_file()]
        for ff in _files:
            if not ff.name.endswith('.hpp') and not ff.name.endswith('.h'):
                continue
            copy2(str(ff), _framework_header_dir)

        # Copy binaries
        self.tag_log("Framework : Copying binary  ...")
        BuildEnv.mkdir_p(_framework_dir)
        _lib_src_file = '{}/libzmq.a'.format(self.env.install_lib_path)
        _lib_dst_file = '{}/zeromq'.format(_framework_dir)
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

    def _patch_libzmq(self):
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        os.chdir('..')

        # Use patch script
        # https://github.com/techtonik/python-patch
        cmd = "python {}/patch.py {}/libzmq.patch".format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
        os.chdir(build_path)
        self.tag_log("Patched")
