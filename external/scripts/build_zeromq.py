#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform

class Builder_zeromq:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://github.com/zeromq/libzmq/releases/download/v4.2.1/zeromq-4.2.1.tar.gz'
		self.package_name = 'zeromq'
		self.archive_file = 'zeromq-4.2.1.tar.gz'

		self.subpkg_url = 'https://github.com/zeromq/cppzmq/archive/v4.2.2.tar.gz'
		self.subpkg_name = 'cppzmq'
		self.subpkg_archive = 'cppzmq-4.2.2.tar.gz'

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
		self.env.download_file(self.subpkg_url, self.subpkg_archive)

		print("  [#2] Extracting package")
		self.env.extract_tarball(self.archive_file, self.package_name)
		self.env.extract_tarball(self.subpkg_archive, self.subpkg_name)

		# Patch
		self._patch_libzmq_linux()

	def _post_build(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)

		# There is no install rule, just copy library file into built directory.
		copy2('{}/lib/libzmq-static.a'.format(build_path), self.env.output_lib_path)

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
			if os.path.exists(self.env.output_lib_path+'/libzmq-static.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print("    [{}] Start building ...".format(self.package_name))
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_parent_path)

				os.chdir(build_path)
				os.system('{} cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR={} -DCMAKE_INSTALL_INCLUDEDIR={} ; make libzmq-static -j {}'.format(
					self.env.BUILD_FLAG,
					self.env.output_lib_path,
					self.env.output_include_path,
					self.env.NJOBS
				))

	def _patch_libzmq_linux(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		os.chdir('..')

		# Use patch script
		# https://github.com/techtonik/python-patch
		os.system("{}/patch.py {}/libzmq.patch".format(
			self.env.working_path,
			self.env.patch_path
		))
		os.chdir(build_path)
		print("    [{}] Patched".format(self.package_name))
