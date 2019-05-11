#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import zeromqWindowsBuilder
from .linux import zeromqLinuxBuilder

class Builder_zeromq(PackageBuilder):
    def __init__(self):
        super(Builder_zeromq, self)
        self.package = {
            'name': 'zeromq',
            'url': 'https://github.com/zeromq/libzmq/releases/download/v4.3.1/zeromq-4.3.1.tar.gz',
            'filename': 'zeromq-4.3.1.tar.gz'
        }
        self.builder = {
            Platform.Windows: zeromqWindowsBuilder(
                self.package,
                {
                    'checker': 'libzmq.lib'
                }
            ),
            Platform.Linux: zeromqLinuxBuilder(
                self.package,
                {
                    'checker': 'libzmq-static.a'
                }
            ),
            Platform.macOS: zeromqLinuxBuilder(
                self.package,
                {
                    'checker': 'zmq-static.dylib'
                }
            )
        }
