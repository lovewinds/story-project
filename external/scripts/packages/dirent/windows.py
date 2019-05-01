#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class direntWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        # Compiled with main code, so additional steps are not required.
        pass
