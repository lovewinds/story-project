#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform

class Builder_g3log:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://github.com/KjellKod/g3log/archive/1.3.tar.gz'
		self.package_name = 'g3log'
		self.archive_file = 'g3log-1.3.tar.gz'

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

		# Patch
		self.patch_g3log_remove_warnings()

	def _post_build(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)

		# There is no install rule, just copy library file into built directory.
		copy2('{}/libg3logger.a'.format(build_path), self.env.output_lib_path)
		if os.path.exists('{}/libg3logger.so'.format(build_path)):
			copy2('{}/libg3logger.so'.format(build_path), self.env.output_lib_path)
		if os.path.exists('{}/libg3logger_shared.dylib'.format(build_path)):
			copy2('{}/libg3logger_shared.dylib'.format(build_path), self.env.output_lib_path)

	def _do_build(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.iOS):
			if os.path.exists(self.env.output_lib_path+'/libg3logger.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print("    [{}] Start building ...".format(self.package_name))
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_path)
				os.system('{} cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE={} ..; make -j {} g3logger; make -j {} g3logger_shared'.format(
					self.env.BUILD_FLAG,
					self.env.BUILD_CONF,
					self.env.NJOBS,
					self.env.NJOBS
				))

	def patch_g3log_remove_warnings(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		os.chdir('..')

		# Use patch script
		# https://github.com/techtonik/python-patch
		os.system("{}/patch.py {}/g3log-1.2-remove_warnings.patch".format(
			self.env.working_path,
			self.env.patch_path
		))
		os.chdir(build_path)
		print("    [{}] Patched".format(self.package_name))
