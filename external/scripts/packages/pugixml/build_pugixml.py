#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import pugixmlWindowsBuilder
from .linux import pugixmlLinuxBuilder

class Builder_pugixml(PackageBuilder):
    def __init__(self):
        super(Builder_pugixml, self)
        self.package = {
            'name': 'pugixml',
            'url': 'http://github.com/zeux/pugixml/releases/download/v1.7/pugixml-1.7.tar.gz',
            'filename': 'pugixml-1.7.tar.gz'
        }
        self.builder = {
            Platform.Windows: pugixmlWindowsBuilder(
                self.package,
                {
                    'checker': 'pugixml.lib'
                }
            ),
            Platform.Linux: pugixmlLinuxBuilder(
                self.package,
                {
                    'checker': 'libpugixml.a'
                }
            ),
            Platform.macOS: pugixmlLinuxBuilder(
                self.package,
                {
                    'checker': 'pugixml.dylib'
                }
            )
        }
