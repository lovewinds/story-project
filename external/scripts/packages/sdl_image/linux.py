#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
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
        self.ext_build_png()
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

        # self.env.output_lib_path+'/libSDL2_image.a'
        _check = f'{self.env.output_lib_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(pkg_path)
        cmd = './autogen.sh --prefix={}'.format(
            self.env.output_bin_path,
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.output_bin_path,
            self.env.output_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])



    def ext_build_jpeg(self):
        ext_path = '{}/{}/external'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/external/jpeg-9/build'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.output_lib_path+'/libjpeg.a'):
            print("       [{}] already built.".format('libjpeg'))
            return

        print("       [{}] Start building ...".format('libjpeg'))
        # Apply patch
        os.chdir(ext_path)
        cmd = "{}/patch.py jpeg-9.patch".format(
            self.env.working_path
        )
        self.env.run_command(cmd, module_name='libjpeg')

        # Build
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.output_bin_path,
            self.env.output_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name='libjpeg')

    def ext_build_png(self):
        ext_path = '{}/{}/external'.format(
            self.env.source_path,
            self.config['name']
        )
        build_path = '{}/{}/external/libpng-1.6.2/build'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.output_lib_path+'/libpng16.a'):
            print("       [{}] already built.".format('libpng'))
            return

        print("       [{}] Start building ...".format('libpng'))
        # Apply patch
        os.chdir(ext_path)
        cmd = "{}/patch.py libpng-1.6.2.patch".format(
            self.env.working_path
        )
        self.env.run_command(cmd, module_name='libpng')

        # Build
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.output_bin_path,
            self.env.output_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name='libpng')
