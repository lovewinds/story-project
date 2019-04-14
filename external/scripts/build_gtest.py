#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

class Builder_gtest(PackageBuilder):
	def __init__(self):
		super(Builder_gtest, self)
		self.setup = {
			'name': 'gtest',
			'common': {
				'url': 'https://github.com/google/googletest/archive/release-1.8.0.tar.gz',
				'filename': 'googletest-release-1.8.0.tar.gz'
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
		if os.path.exists(self.env.output_lib_path+'/libgtest.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		# os.chdir(build_parent_path)
		os.chdir(build_path)
		# make install
		#  -DCMAKE_INSTALL_LIBDIR={} -DCMAKE_INSTALL_INCLUDEDIR={}
			# self.env.output_lib_path,
			# self.env.output_include_path,
		cmd = '{} cmake -DCMAKE_INSTALL_PREFIX={} ..; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
			self.env.NJOBS,
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
