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

    def build(self):
        build_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )

        # Check 'Python.framework'
        _check = self.env.framework_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = f'make iOS'
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()

        archive_path = self.env.source_path / self.config['name'] / 'dist'

        # Check 'Python.framework'
        _check = self.env.framework_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already installed.")
            return

        # Extract archive first
        os.chdir(archive_path)
        tarfile.open('Python-3.7-iOS-support.b3.tar.gz').extractall(archive_path)
        # os.chmod('{}/Python/libPython.a'.format(path_framework), 0o755)

        self.tag_log("Installing frameworks ...")
        self.install_bzip2(archive_path)
        self.install_openssl(archive_path)
        self.install_xz(archive_path)
        self.install_python3(archive_path)


    def install_bzip2(self, archive_path):
        if not os.path.isdir(self.env.framework_path / 'BZip2.framework'):
            self.tag_log("    BZip2")
            copytree(archive_path / 'BZip2',
                self.env.framework_path / 'BZip2.framework')
            target_framework = self.env.framework_path / 'BZip2.framework'

            # Rename binary
            os.rename(target_framework / 'libbzip2.a', target_framework / 'BZip2')

            # Create plist
            dir_resource = target_framework / 'Resources'
            BuildEnv.mkdir_p(dir_resource)
            plist_str = self.env.apple_framework_plist.replace(
                '${FRAMEWORK_NAME}', 'BZip2'
            ).replace(
                '${FRAMEWORK_CURRENT_VERSION}', '1.0.8'
            )
            plist_file = '{}/Info.plist'.format(dir_resource)
            with open(plist_file, "w") as pf:
                pf.write(plist_str)

    def install_openssl(self, archive_path):
        if not os.path.isdir(self.env.framework_path / 'OpenSSL.framework'):
            self.tag_log("    OpenSSL")
            copytree(archive_path / 'OpenSSL',
                self.env.framework_path / 'OpenSSL.framework')
            target_framework = self.env.framework_path / 'OpenSSL.framework'

            # Rename binary
            os.rename(target_framework / 'libOpenSSL.a', target_framework / 'OpenSSL')

            # Create plist
            dir_resource = target_framework / 'Resources'
            BuildEnv.mkdir_p(dir_resource)
            plist_str = self.env.apple_framework_plist.replace(
                '${FRAMEWORK_NAME}', 'OpenSSL'
            ).replace(
                '${FRAMEWORK_CURRENT_VERSION}', '1.1.1d'
            )
            plist_file = '{}/Info.plist'.format(dir_resource)
            with open(plist_file, "w") as pf:
                pf.write(plist_str)

    def install_xz(self, archive_path):
        if not os.path.isdir(self.env.framework_path / 'XZ.framework'):
            self.tag_log("    XZ")
            copytree(archive_path / 'XZ',
                self.env.framework_path / 'XZ.framework')
            target_framework = self.env.framework_path / 'XZ.framework'

            # Rename binary
            os.rename(target_framework / 'libxz.a', target_framework / 'XZ')

            # Create plist
            dir_resource = target_framework / 'Resources'
            BuildEnv.mkdir_p(dir_resource)
            plist_str = self.env.apple_framework_plist.replace(
                '${FRAMEWORK_NAME}', 'XZ'
            ).replace(
                '${FRAMEWORK_CURRENT_VERSION}', '5.2.4'
            )
            plist_file = '{}/Info.plist'.format(dir_resource)
            with open(plist_file, "w") as pf:
                pf.write(plist_str)

    def install_python3(self, archive_path):
        if not os.path.isdir(self.env.framework_path / 'Python.framework'):
            self.tag_log("    Python3")
            copytree(archive_path / 'Python',
                self.env.framework_path / 'Python.framework')
            target_framework = self.env.framework_path / 'Python.framework'

            # Rename binary
            os.rename(target_framework / 'libPython.a', target_framework / 'Python')

            # Create plist
            dir_resource = target_framework / 'Resources'
            BuildEnv.mkdir_p(dir_resource)
            plist_str = self.env.apple_framework_plist.replace(
                '${FRAMEWORK_NAME}', 'Python'
            ).replace(
                '${FRAMEWORK_CURRENT_VERSION}', '3.7.5'
            )
            plist_file = '{}/Info.plist'.format(dir_resource)
            with open(plist_file, "w") as pf:
                pf.write(plist_str)
