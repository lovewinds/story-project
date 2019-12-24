#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2ImageLinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        self.ext_build_zlib()
        self.ext_build_jpeg()

    def build(self):
        pkg_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        # self.env.install_lib_path+'/libSDL2_image.a'
        _check = f'{self.env.install_lib_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(pkg_path)
        cmd = './autogen.sh --prefix={}'.format(
            self.env.install_bin_path,
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])



    def ext_build_jpeg(self):
        ext_path = '{}/{}/external'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/external/jpeg-9b/build'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.install_lib_path / 'libjpeg.a'):
            print("       [{}] already built.".format('libjpeg'))
            return

        print("       [{}] Start building ...".format('libjpeg'))
        # Apply patch
        os.chdir(ext_path)
        cmd = "{}/patch.py jpeg-9b.patch".format(
            self.env.working_path
        )
        self.env.run_command(cmd, module_name='libjpeg')

        # Build
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name='libjpeg')

    def ext_build_zlib(self):
        ext_path = '{}/{}/external'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/external/zlib-1.2.11/build'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.install_lib_path / 'libz.dylib'):
            print("       [{}] already built.".format('zlib'))
            return

        print("       [{}] Start building ...".format('zlib'))
        # Apply patch
        os.chdir(ext_path)
        cmd = "{}/patch.py zlib-1.2.11.patch".format(
            self.env.working_path
        )
        self.env.run_command(cmd, module_name='zlib')

        # Build
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name='zlib')
