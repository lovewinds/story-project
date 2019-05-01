#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class zeromqLinuxBuilder(PlatformBuilder):
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

        # if os.path.exists(self.env.install_lib_path+'/libzmq-static.a'):
        _check = f'{self.env.install_lib_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX={} ; make -j {} libzmq-static; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])

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
