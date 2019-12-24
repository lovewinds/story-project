#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .linux import libPNGLinuxBuilder

class Builder_libpng(PackageBuilder):
    def __init__(self):
        super(Builder_libpng, self)
        self.package = {
            'name': 'libpng',
            'url': 'http://prdownloads.sourceforge.net/libpng/libpng-1.6.37.tar.gz?download',
            'filename': 'libpng-1.6.37.tar.gz'
        }
        self.builder = {
            Platform.Linux: libPNGLinuxBuilder(
                self.package,
                {
                    'checker': 'libpng16.so'
                }
            ),
            Platform.macOS: libPNGLinuxBuilder(
                self.package,
                {
                    'checker': 'libpng16.dylib'
                }
            )
        }
