#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import g3logWindowsBuilder
from .linux import g3logLinuxBuilder

class Builder_g3log(PackageBuilder):
    def __init__(self):
        super(Builder_g3log, self)
        self.package = {
            'name': 'g3log',
            'url': 'https://github.com/KjellKod/g3log/archive/1.3.tar.gz',
            'filename': 'g3log-1.3.tar.gz'
        }
        # self.setup = {
        # 	'Linux': {
        # 		'pre': self.patch_g3log_remove_warnings,
        # 		'build': self._build_Linux,
        # 		'post': self._post_build_Linux,
        # 	},
        # 	'macOS': {
        # 		'pre': self.patch_g3log_remove_warnings,
        # 		'build': self._build_Linux,
        # 		'post': self._post_build_Linux,
        # 	},
        # 	'iOS': {
        # 		'pre': None,
        # 		'build': None,
        # 		'post': None,
        # 	}
        # }

        self.builder = {
            Platform.Windows: g3logWindowsBuilder(
                self.package,
                {
                    'checker': 'g3log.lib'
                }
            ),
            Platform.Linux: g3logLinuxBuilder(
                self.package,
                {
                    'checker': 'libg3log.a'
                }
            ),
            Platform.macOS: g3logLinuxBuilder(
                self.package,
                {
                    'checker': 'g3log.dylib'
                }
            )
        }
