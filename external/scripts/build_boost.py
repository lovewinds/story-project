#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_boost(Builder):
	def __init__(self):
		super(Builder_boost, self)
		self.setup = {
			'name': 'boost',
			'common': {
				'url': 'https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz',
				'filename': 'boost_1_64_0.tar.gz'
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
		pkg_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libboost_python3.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(pkg_path)
		# self.env.BUILD_FLAG,
		cmd = '{} PYTHON=python3; PATH={}:$PATH; ./bootstrap.sh --with-python-root={} --with-libraries=python --prefix={}'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.output_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

		cmd = '{} ./b2 --build-dir=build --prefix={} --with-python link=static cxxflags="-fPIC" linkflags="-fPIC" install'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
