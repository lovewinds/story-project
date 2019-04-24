#!/usr/bin/env python3
import os
import tarfile
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pythoniOSBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        self._patch_iphonesimulator()
        self._custom_build_python_macOS()
        self._create_symlink()

    def build(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )

        # if os.path.exists(self.env.framework_path+'/Python.framework'):
        _check = f'{self.env.framework_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("[iOS] already built.")
            return

        self.tag_log("[iOS] Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = 'make iOS'
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()
        pkg_path = '{}/{}/dist'.format(
            self.env.source_path,
            self.config['name']
        )
        path_framework = '{}/Python.framework'.format(
            self.env.framework_path,
        )

        # if os.path.exists(self.env.framework_path+'/Python.framework'):
        _check = f'{self.env.framework_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("[iOS] already installed.")
            return

        self.tag_log("[iOS] Installing framework ...")
        BuildEnv.mkdir_p(self.env.framework_path)
        os.chdir(pkg_path)
        tarfile.open('Python-3.6-iOS-support.b6.tar.gz').extractall(path_framework)
        # os.rename('{}/Python-3.6-iOS-support.b6'.format(self.env.framework_path),
        # 		  '{}/Python.framework'.format(self.env.framework_path))
        os.chmod('{}/Support/Python/libPython.a'.format(path_framework), 0o755)



    def _patch_iphonesimulator(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        self.tag_log("[iOS] Patching python for iOS ...")
        os.chdir(build_path)
        # cmd = 'patch -p1 Makefile < {}/python_ios.patch'.format(
        # 	self.env.patch_path
        # )
        cmd = 'python {}/patch.py {}/python_ios.patch'.format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])
        self.tag_log("[iOS] Patched")

    def _custom_build_python_macOS(self):
        '''Build host python executable first to compile iOS version.
        '''
        self.tag_log("Downloading python ...")
        package_url = self.config['common']['url']
        package_file = self.config['common']['filename']
        self.env.download_file(package_url, package_file)

        self.tag_log("Extracting ...")
        self.env.extract_tarball(package_file, 'python_bin')

        self.tag_log("Building python binary first ...")
        build_path = '{}/python_bin/build'.format(
            self.env.source_path
        )
        if os.path.exists(self.env.output_bin_path+'/python3'):
            self.tag_log("[macOS] Already built.")
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

    def _create_symlink(self):
        '''Create symbolic link (required to build boost)
        '''
        os.chdir(self.env.output_include_path)
        if not os.path.exists('python3.6'):
            cmd = 'ln -s python3.6m python3.6'
            self.env.run_command(cmd, module_name=self.config['name'])

        os.chdir(self.env.output_bin_path)
        if not os.path.exists('python'):
            cmd = 'ln -s python3 python'
            self.env.run_command(cmd, module_name=self.config['name'])
