#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import pybind11WindowsBuilder
from .linux import pybind11LinuxBuilder

class Builder_pybind11(PackageBuilder):
    def __init__(self):
        super()
        self.package = {
            'name': 'pybind11',
            'url': 'https://github.com/pybind/pybind11/archive/v2.2.4.tar.gz',
            'filename': 'pybind11-2.2.4.tar.gz'
        }
        self.builder = {
            Platform.Windows: pybind11WindowsBuilder(
                self.package,
                {
                    'checker': 'pybind11.lib'
                }
            ),
            Platform.Linux: pybind11LinuxBuilder(
                self.package,
                {
                    'checker': '../include/pybind11'
                }
            ),
            Platform.macOS: pybind11LinuxBuilder(
                self.package,
                {
                    'checker': 'pybind11.dylib'
                }
            )
        }
