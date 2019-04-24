#!/usr/bin/python
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import SDL2ImageWindowsBuilder
from .linux import SDL2ImageLinuxBuilder
from .ios import SDL2ImageiOSBuilder

class Builder_SDL2_Image(PackageBuilder):
    def __init__(self):
        super(Builder_SDL2_Image, self)
        self.package = {
            'name': 'SDL2_image',
            'url': 'https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.1.tar.gz',
            'filename': 'SDL2_image-2.0.1.tar.gz'
        }
        self.builder = {
            Platform.Windows: SDL2ImageWindowsBuilder(
                self.package,
                {
                    'checker': 'SDL2_image.lib'
                }
            ),
            Platform.Linux: SDL2ImageLinuxBuilder(
                self.package,
                {
                    'checker': 'libSDL2_image.so'
                }
            ),
            Platform.macOS: SDL2ImageLinuxBuilder(
                self.package,
                {
                    'checker': 'SDL2_image.dylib'
                }
            ),
            Platform.iOS: SDL2ImageiOSBuilder(
                self.package,
                {
                    'checker': 'libSDL2_image.a'
                }
            )
        }
