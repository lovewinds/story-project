#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_SDL2_TTF(Builder):
	def __init__(self):
		super(Builder_SDL2_TTF, self)
		self.setup = {
			'name': 'SDL2_ttf',
			'common': {
				'url': 'https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz',
				'filename': 'SDL2_ttf-2.0.14.tar.gz'
			},
			'Linux': {
				'pre': self._do_build_freetype,
				'build': self._build_Linux,
				'post': None,
			},
			'macOS': {
				'pre': self._do_build_freetype,
				'build': self._build_Linux,
				'post': None,
			},
			'iOS': {
				'pre': None,
				'build': None,
				'post': None,
			}
		}

	def _build_Linux(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libSDL2_ttf.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def _do_build_freetype(self):
		build_path = '{}/{}/external/freetype-2.4.12/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_bin_path+'/freetype-config'):
			print("       [FreeType2] already built.")
			return

		print("       [FreeType] Start building ...")
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name='FreeType')
