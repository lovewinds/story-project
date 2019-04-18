#!/usr/bin/python
import os
import shutil
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

# from .windows import boostWindowsBuilder
from .linux import boostLinuxBuilder
from .ios import boostiOSBuilder

class Builder_boost(PackageBuilder):
    def __init__(self):
        super(Builder_boost, self)
        self.package = {
            'name': 'boost',
            'url': 'https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz',
            'filename': 'boost_1_64_0.tar.gz'
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
        # 		'url': 'https://github.com/faithfracture/Apple-Boost-BuildScript/archive/master.zip',
        # 		'filename': 'Apple-Boost-BuildScript-master.zip',
        # 		'pre': self.patch_boost_apple,
        # 		'build': self._build_iOS,
        # 		'post': self.post_build_iOS,
        # 	}
        # }

        self.builder = {
            # Platform.Windows: boostWindowsBuilder(
            #     self.package,
            #     {
            #         'checker': 'libboost.lib'
            #     }
            # ),
            Platform.Linux: boostLinuxBuilder(
                self.package,
                {
                    'checker': 'libboost_python3.a'
                }
            ),
            Platform.macOS: boostLinuxBuilder(
                self.package,
                {
                    'checker': 'libboost_python3.a'
                }
            ),
            Platform.iOS: boostiOSBuilder(
                self.package,
                {
                    'url': 'https://github.com/faithfracture/Apple-Boost-BuildScript/archive/master.zip',
                    'filename': 'Apple-Boost-BuildScript-master.zip',
                    'checker': 'boost.framework'
                }
            )
        }
