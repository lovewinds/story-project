#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

# from .windows import icuWindowsBuilder
from .linux import icuLinuxBuilder


class Builder_icu(PackageBuilder):
    def __init__(self):
        super(Builder_icu, self)
        self.package = {
            'name': 'icu',
            'url': 'https://github.com/unicode-org/icu/releases/download/release-66-1/icu4c-66_1-src.tgz',
            'filename': 'icu4c-66.1.tgz'
        }
        self.builder = {
            Platform.Linux: icuLinuxBuilder(
                self.package,
                {
                    'checker': 'libicui18n.so'
                }
            ),
            Platform.macOS: icuLinuxBuilder(
                self.package,
                {
                    'checker': 'libicui18n.dylib'
                }
            ),
            Platform.iOS: icuLinuxBuilder(
                self.package,
                {
                    'checker': 'libicui18n.dylib'
                }
            )
        }
