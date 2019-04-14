#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

class Builder_protobuf(PackageBuilder):
	def __init__(self):
		super(Builder_protobuf, self)
		self.setup = {
			'name': 'protobuf',
			'common': {
				'url': 'https://github.com/google/protobuf/archive/v3.0.0.tar.gz',
				'filename': 'protobuf-3.0.0.tar.gz'
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
		build_path = '{}/{}/cmake/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libprotobuf.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX={} ..; make -j {} libprotobuf; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
