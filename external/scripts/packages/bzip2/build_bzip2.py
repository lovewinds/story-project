#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .linux import bzip2LinuxBuilder

class Builder_bzip2(PackageBuilder):
    def __init__(self):
        super(Builder_bzip2, self)
        self.package = {
            'name': 'bzip2',
            'url': 'https://sourceware.org/pub/bzip2/bzip2-1.0.6.tar.gz',
            'filename': 'bzip2-1.0.6.tar.gz'
        }
        self.builder = {
            Platform.Linux: bzip2LinuxBuilder(
                self.package,
                {
                    'checker': 'libbz2.a'
                }
            )
        }
