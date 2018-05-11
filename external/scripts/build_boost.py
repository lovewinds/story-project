#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform

class Builder_boost:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz'
		self.package_name = 'boost'
		self.archive_file = 'boost_1_64_0.tar.gz'

	def build(self, env_param):
		print("Building {} ...".format(self.package_name))
		self.env = env_param
		self._pre_build()
		self._do_build()
		self._post_build()

	def _pre_build(self):
		print("  [#0] Checking build output exists")

		print("  [#1] Downloading package")
		self.env.download_file(self.package_url, self.archive_file)

		print("  [#2] Extracting package")
		self.env.extract_tarball(self.archive_file, self.package_name)

	def _post_build(self):
		pass

	def _do_build(self):
		pkg_path = '{}/{}'.format(
			self.env.source_path,
			self.package_name
		)
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.macOS or
			self.env.platform == Platform.iOS):
			if os.path.exists(self.env.output_lib_path+'/libboost_python3.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print("    [{}] Start building ...".format(self.package_name))
				BuildEnv.mkdir_p(build_path)
				os.chdir(pkg_path)
				# self.env.BUILD_FLAG,
				os.system('PYTHON=python3; PATH={}:$PATH; ./bootstrap.sh --with-python-root={} --with-libraries=python --prefix={}'.format(
					self.env.output_bin_path,
					self.env.output_path,
					self.env.output_path
				))
				os.system("./b2 --build-dir=build --cxxflags='-fPIC' --prefix={} --with-python link=static install".format(
					self.env.output_path,
				))
