#!/usr/bin/env python3
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

from .windows import SDL2WindowsBuilder
from .linux import SDL2LinuxBuilder
from .ios import SDL2iOSBuilder

class Builder_SDL2(PackageBuilder):
	def __init__(self):
		super(Builder_SDL2, self)
		self.package = {
			'name': 'SDL2',
			'url': 'https://www.libsdl.org/release/SDL2-2.0.9.tar.gz',
			'filename': 'SDL2-2.0.9.tar.gz'
		}
		self.builder = {
			Platform.Windows: SDL2WindowsBuilder(
				self.package,
				{
					'checker': 'SDL2.lib'
				}
			),
			Platform.Linux: SDL2LinuxBuilder(
				self.package,
				{
					'checker': 'libSDL2.so'
				}
			),
			Platform.macOS: SDL2LinuxBuilder(
				self.package,
				{
					'checker': 'libSDL2.dylib'
				}
			),
			Platform.iOS: SDL2iOSBuilder(
				self.package,
				{
					'checker': 'libSDL2.a'
				}
			)
		}
