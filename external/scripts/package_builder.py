#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class PackageBuilder(object):
    def __init__(self):
        self.env = None
        self.package = {}
        self.builder = {}

    def build(self, build_env):
        print("Building [{}] ...".format(self.package['name']))
        self.env = build_env
        self.build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.package['name']
        )

        # Check platform and trigger matching builder
        _builder = self.builder.get(self.env.platform)
        if _builder is not None:
            _builder.configure(self.env)
            _builder.pre()
            _builder.build()
            _builder.post()

        return True