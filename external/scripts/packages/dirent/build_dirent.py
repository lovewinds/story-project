#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import direntWindowsBuilder

class Builder_dirent(PackageBuilder):
    def __init__(self):
        super(Builder_dirent, self)
        self.package = {
            'name': 'dirent',
            'url': 'https://github.com/tronkko/dirent/archive/1.23.2.tar.gz',
            'filename': 'dirent-1.23.2.tar.gz'
        }
        self.builder = {
            Platform.Windows: direntWindowsBuilder(self.package)
        }
