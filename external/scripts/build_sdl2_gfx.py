#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_SDL2_gfx(Builder):
	def __init__(self):
		super(Builder_SDL2_gfx, self)
		self.setup = {
			'name': 'SDL2_gfx',
			'common': {
				'url': 'http://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-1.0.4.tar.gz',
				'filename': 'SDL2_gfx-1.0.4.tar.gz'
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
		build_parent_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libSDL2_gfx.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_parent_path)
		cmd = '{} PATH={}:$PATH ./autogen.sh --prefix={}'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
