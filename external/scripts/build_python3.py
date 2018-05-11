#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform

class Builder_python:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://www.python.org/ftp/python/3.6.1/Python-3.6.1.tgz'
		self.package_name = 'python'
		self.archive_file = 'Python-3.6.1.tgz'

	def build(self, env_param):
		print("Building {} ...".format(self.package_name))
		self.env = env_param
		self._pre_build()
		self._do_build()
		self._post_build()

	def _pre_build(self):
		print("  [#0] Checking build output exists")

		print("  [#1] Downloading package")
		# Download process is not finished with content type issue.
		headers = {'Content-Type': 'application/x-gzip; charset=utf-8'}
		self.env.download_file(self.package_url, self.archive_file)

		print("  [#2] Extracting package")
		self.env.extract_tarball(self.archive_file, self.package_name)

		# Patch

	def _post_build(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)

		# There is no install rule, just copy library file into built directory.
		os.system('ln -s {}/python3.6m {}/python3.6'.format(
			self.env.output_include_path,
			self.env.output_include_path
		))
		os.system('ln -s {}/python3 {}/python'.format(
			self.env.output_bin_path,
			self.env.output_bin_path
		))

	def _do_build(self):
		build_parent_path = '{}/{}'.format(
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
			if os.path.exists(self.env.output_lib_path+'/libpython3.6m.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print("    [{}] Start building ...".format(self.package_name))
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_parent_path)

				os.chdir(build_path)
				os.system('{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
					self.env.BUILD_FLAG,
					self.env.output_bin_path,
					self.env.output_path,
					self.env.NJOBS
				))
