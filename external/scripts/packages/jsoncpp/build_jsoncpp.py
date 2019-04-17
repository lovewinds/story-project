#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import jsoncppWindowsBuilder
from .linux import jsoncppLinuxBuilder

class Builder_jsoncpp(PackageBuilder):
    def __init__(self):
        super(Builder_jsoncpp, self)
        self.package = {
            'name': 'jsoncpp',
            'url': 'https://github.com/open-source-parsers/jsoncpp/archive/1.6.5.tar.gz',
            'filename': 'jsoncpp-1.6.5.tar.gz'
        }
        self.builder = {
            Platform.Windows: jsoncppWindowsBuilder(
                self.package,
                {
                    'checker': 'jsoncpp.lib'
                }
            ),
            Platform.Linux: jsoncppLinuxBuilder(
                self.package,
                {
                    'checker': 'jsoncpp.so'
                }
            ),
            Platform.macOS: jsoncppLinuxBuilder(
                self.package,
                {
                    'checker': 'jsoncpp.dylib'
                }
            )
        }
