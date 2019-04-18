#!/usr/bin/python
import os
import shutil
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

class Builder_boost(PackageBuilder):
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
				'url': 'https://github.com/faithfracture/Apple-Boost-BuildScript/archive/master.zip',
				'filename': 'Apple-Boost-BuildScript-master.zip',
				'pre': self.patch_boost_apple,
				'build': self._build_iOS,
				'post': self.post_build_iOS,
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
			self.env.output_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def patch_boost_apple(self):
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		os.chdir(build_path)
		# cmd = '{}/patch.py {}/boost-use-python3.patch'.format(
		# 	self.env.working_path,
		# 	self.env.patch_path
		# )
		cmd = 'patch -t -p1 boost.sh < {}/boost-use-python3.patch'.format(
			self.env.patch_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
		print("       [{}] Patched".format(self.setup['name']))

	def _build_iOS(self):
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.framework_path+'/boost.framework'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		os.system('chmod +x boost.sh')
		# /Users/ariens/source/story-project/external/built
		cmd = '{} ./boost.sh -ios --boost-version 1.64.0 --boost-libs python --python-root {} --min-ios-version 7.0'.format(
			self.env.BUILD_FLAG,
			self.env.output_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def post_build_iOS(self):
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)

		# Copy built framework
		if not os.path.exists(self.env.framework_path+'/boost.framework'):
			copytree(
				'{}/build/boost/1.64.0/ios/framework/boost.framework'.format(build_path),
				'{}/boost.framework'.format(self.env.framework_path)
			)
