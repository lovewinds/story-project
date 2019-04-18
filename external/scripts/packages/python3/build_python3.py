#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

# from .windows import pythonWindowsBuilder
from .linux import pythonLinuxBuilder
from .ios import pythoniOSBuilder

class Builder_python(PackageBuilder):
    def __init__(self):
        super(Builder_python, self)
        self.package = {
            'name': 'python',
            'url': 'https://www.python.org/ftp/python/3.6.1/Python-3.6.1.tgz',
            'filename': 'Python-3.6.1.tgz'
        }
        # 	'Linux': {
        # 		'pre': None,
        # 		'build': self._build_Linux,
        # 		'post': self._post_build,
        # 	},
        # 	'macOS': {
        # 		'pre': None,
        # 		'build': self._build_Linux,
        # 		'post': self._post_build,
        # 	},
        # 	'iOS': {
        # 		'url': 'https://github.com/pybee/Python-Apple-support/archive/3.6-b6.tar.gz',
        # 		'filename': 'Python-Apple-support-3.6-b6.tar.gz',
        # 		'pre': self._pre_build_iOS,
        # 		'build': self._build_iOS,
        # 		'post': self._post_build_iOS,
        # 	}
        # }

        self.builder = {
            # Platform.Windows: pythonWindowsBuilder(
            #     self.package,
            #     {
            #         'checker': 'libpython.lib'
            #     }
            # ),
            Platform.Linux: pythonLinuxBuilder(
                self.package,
                {
                    'checker': 'libpython3.6m.a'
                }
            ),
            Platform.iOS: pythoniOSBuilder(
                self.package,
                {
                    'url': 'https://github.com/pybee/Python-Apple-support/archive/3.6-b6.tar.gz',
                    'filename': 'Python-Apple-support-3.6-b6.tar.gz',
                    'checker': 'Python.framework'
                }
            )
        }
