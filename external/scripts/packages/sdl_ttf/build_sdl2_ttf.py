#!/usr/bin/python
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import SDL2TTFWindowsBuilder
from .linux import SDL2TTFLinuxBuilder
from .ios import SDL2TTFiOSBuilder

class Builder_SDL2_TTF(PackageBuilder):
    def __init__(self):
        super(Builder_SDL2_TTF, self)
        self.package = {
            'name': 'SDL2_ttf',
            'url': 'https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz',
            'filename': 'SDL2_ttf-2.0.14.tar.gz'
        }
        self.builder = {
            Platform.Windows: SDL2TTFWindowsBuilder(
                self.package,
                {
                    'checker': 'SDL2_ttf.lib'
                }
            ),
            Platform.Linux: SDL2TTFLinuxBuilder(
                self.package,
                {
                    'checker': 'libSDL2_ttf.so'
                }
            ),
            Platform.macOS: SDL2TTFLinuxBuilder(
                self.package,
                {
                    'checker': 'SDL2_ttf.dylib'
                }
            ),
            Platform.iOS: SDL2TTFiOSBuilder(
                self.package,
                {
                    'checker': 'libSDL2_ttf.a'
                }
            )
        }
        # self.config = {
        #     'name': 'SDL2_ttf',
        #     'common': {
        #         'url': 'https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz',
        #         'filename': 'SDL2_ttf-2.0.14.tar.gz'
        #     },
        #     'Linux': {
        #         'pre': self._do_build_freetype,
        #         'build': self._build_Linux,
        #         'post': None,
        #     },
        #     'macOS': {
        #         'pre': self._do_build_freetype,
        #         'build': self._build_Linux,
        #         'post': None,
        #     },
        #     'iOS': {
        #         'pre': None,
        #         'build': self.build_iOS,
        #         'post': self.post_build_iOS,
        #     }
        # }