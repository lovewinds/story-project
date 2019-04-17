#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import gtestWindowsBuilder
from .linux import gtestLinuxBuilder

class Builder_gtest(PackageBuilder):
    def __init__(self):
        super(Builder_gtest, self)
        self.package = {
            'name': 'gtest',
            'url': 'https://github.com/google/googletest/archive/release-1.8.0.tar.gz',
            'filename': 'googletest-release-1.8.0.tar.gz'
        }

        # 	'Linux': {
        # 		'pre': None,
        # 		'build': self._build_Linux,
        # 		'post': None,
        # 	},
        # 	'macOS': {
        # 		'pre': None,
        # 		'build': self._build_Linux,
        # 		'post': None,
        # 	},
        # 	'iOS': {
        # 		'pre': None,
        # 		'build': None,
        # 		'post': None,
        # 	}
        # }

        self.builder = {
            Platform.Windows: gtestWindowsBuilder(
                self.package,
                {
                    'checker': 'gtest.lib'
                }
            ),
            Platform.Linux: gtestLinuxBuilder(
                self.package,
                {
                    'checker': 'libgtest.so'
                }
            ),
            Platform.macOS: gtestLinuxBuilder(
                self.package,
                {
                    'checker': 'gtest.dylib'
                }
            )
        }

