#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_SDL2(Builder):
	def __init__(self):
		super(Builder_SDL2, self)
		self.setup = {
			'name': 'SDL2',
			'common': {
				'url': 'https://www.libsdl.org/release/SDL2-2.0.5.tar.gz',
				'filename': 'SDL2-2.0.5.tar.gz'
			},
			'Linux': {
				'pre': None,
				'build': self._build_Linux,
				'post': None,
			},
			'macOS': {
				'pre': None,
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
		if os.path.exists(self.env.output_lib_path+'/libSDL2.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
