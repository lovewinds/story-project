#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .linux import zlibLinuxBuilder
from .windows import zlibWindowsBuilder

class Builder_zlib(PackageBuilder):
    def __init__(self):
        super(Builder_zlib, self)
        self.package = {
            'name': 'zlib',
            'url': 'https://www.zlib.net/zlib-1.2.11.tar.gz',
            'filename': 'zlib-1.2.11.tar.gz'
        }
        self.builder = {
            Platform.Windows: zlibWindowsBuilder(
                self.package,
                {
                    'checker': 'zlib1.dll'
                }
            ),
            Platform.Linux: zlibLinuxBuilder(
                self.package,
                {
                    'checker': 'libz.so'
                }
            ),
            Platform.macOS: zlibLinuxBuilder(
                self.package,
                {
                    'checker': 'libz.dylib'
                }
            )
        }
