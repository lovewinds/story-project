#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pythoniOSBuilder(PlatformBuilder):
    def __init__(self, 
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package)

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]

    def build(self):
        build_path = '{}/{}/Xcode-iOS'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = f'{self.env.output_lib_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PREFIX={} {}/ios-build.sh SDL2_ttf'.format(
            self.env.BUILD_FLAG,
            self.env.output_path,
            self.env.working_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])




    def _patch_iphonesimulator(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        print("       [{}-iOS] Patching python for iOS ...".format(self.config['name']))
        os.chdir(build_path)
        # cmd = 'patch -p1 Makefile < {}/python_ios.patch'.format(
        # 	self.env.patch_path
        # )
        cmd = 'python {}/patch.py {}/python_ios.patch'.format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
        self.tag_log("[iOS] Patched".format(self.config['name']))

    def _custom_build_python_macOS(self):
        print("       [{}] Downloading python ...".format(self.config['name']))
        package_url = self.config['common']['url']
        package_file = self.config['common']['filename']
        self.env.download_file(package_url, package_file)

        print("       [{}] Extracting ...".format(self.config['name']))
        self.env.extract_tarball(package_file, 'python_bin')

        print("       [{}] Building python binary first ...".format(self.config['name']))
        build_path = '{}/python_bin/build'.format(
            self.env.source_path
        )
        if os.path.exists(self.env.output_bin_path+'/python3'):
            print("       [{}] already built.".format(self.config['name']))
            return

        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.output_bin_path,
            self.env.output_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        # Create symlink (required to build boost)
        self._post_build()

    def _pre_build_iOS(self):
        self._patch_iphonesimulator()
        self._custom_build_python_macOS()

    def _build_iOS(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        if os.path.exists(self.env.framework_path+'/Python.framework'):
            print("       [{}-iOS] already built.".format(self.config['name']))
            return

        print("       [{}-iOS] Start building ...".format(self.config['name']))
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = 'make iOS'
        self.env.run_command(cmd, module_name=self.config['name'])

    def _post_build_iOS(self):
        pkg_path = '{}/{}/dist'.format(
            self.env.source_path,
            self.config['name']
        )
        path_framework = '{}/Python.framework'.format(
            self.env.framework_path,
        )

        if os.path.exists(self.env.framework_path+'/Python.framework'):
            print("       [{}-iOS] already installed.".format(self.config['name']))
            return

        print("       [{}-iOS] Installing framework ...".format(self.config['name']))
        BuildEnv.mkdir_p(self.env.framework_path)
        os.chdir(pkg_path)
        tarfile.open('Python-3.6-iOS-support.b6.tar.gz').extractall(path_framework)
        # os.rename('{}/Python-3.6-iOS-support.b6'.format(self.env.framework_path),
        # 		  '{}/Python.framework'.format(self.env.framework_path))
        os.chmod('{}/Support/Python/libPython.a'.format(path_framework), 0o755)
