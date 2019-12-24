#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import SDL2gfxWindowsBuilder
from .linux import SDL2gfxLinuxBuilder
from .ios import SDL2gfxiOSBuilder

class Builder_SDL2_gfx(PackageBuilder):
    def __init__(self):
        super(Builder_SDL2_gfx, self)
        self.package = {
            'name': 'SDL2_gfx',
            'url': 'http://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-1.0.4.tar.gz',
            'filename': 'SDL2_gfx-1.0.4.tar.gz'
        }
        self.builder = {
            Platform.Windows: SDL2gfxWindowsBuilder(
                self.package,
                {
                    'checker': 'SDL2_gfx.lib'
                }
            ),
            Platform.Linux: SDL2gfxLinuxBuilder(
                self.package,
                {
                    'checker': 'libSDL2_gfx.so'
                }
            ),
            Platform.macOS: SDL2gfxLinuxBuilder(
                self.package,
                {
                    'checker': 'libSDL2_gfx.dylib'
                }
            ),
            Platform.iOS: SDL2gfxiOSBuilder(
                self.package,
                {
                    'checker': 'libSDL2_gfx.a'
                }
            )
        }
