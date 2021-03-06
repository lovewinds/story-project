#!/usr/bin/env python3
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import gtestWindowsBuilder
from .linux import gtestLinuxBuilder
from .ios import gtestiOSBuilder

class Builder_gtest(PackageBuilder):
    def __init__(self):
        super(Builder_gtest, self)
        self.package = {
            'name': 'gtest',
            'url': 'https://github.com/google/googletest/archive/release-1.10.0.tar.gz',
            'filename': 'googletest-release-1.10.0.tar.gz'
        }
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
            ),
            Platform.iOS: gtestiOSBuilder(
                self.package,
                {
                    'checker': '../frameworks/gtest.framework'
                }
            )
        }

