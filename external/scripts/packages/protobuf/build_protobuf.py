#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import protobufWindowsBuilder
from .linux import protobufLinuxBuilder
from .ios import protobufiOSBuilder

class Builder_protobuf(PackageBuilder):
    def __init__(self):
        super(Builder_protobuf, self)
        self.package = {
            'name': 'protobuf',
            'url': 'https://github.com/google/protobuf/archive/v3.0.0.tar.gz',
            'filename': 'protobuf-3.0.0.tar.gz'
        }

        self.builder = {
            Platform.Windows: protobufWindowsBuilder(
                self.package,
                {
                    'checker': 'libprotobuf.lib'
                }
            ),
            Platform.Linux: protobufLinuxBuilder(
                self.package,
                {
                    'checker': 'libprotobuf.a'
                }
            ),
            Platform.macOS: protobufLinuxBuilder(
                self.package,
                {
                    'checker': 'protobuf.dylib'
                }
            ),
            Platform.iOS: protobufiOSBuilder(
                self.package,
                {
                    'checker': '../frameworks/protobuf.framework'
                }
            )
        }
