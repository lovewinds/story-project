#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform

class Builder_SDL2:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://www.libsdl.org/release/SDL2-2.0.5.tar.gz'
		self.package_name = 'SDL2'
		self.archive_file = 'SDL2-2.0.5.tar.gz'

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

	def _post_build(self):
		if(self.env.platform == Platform.iOS):
			pkg_path = '{}/{}'.format(
				self.env.output_packaging_path,
				self.package_name
			)
			BuildEnv.mkdir_p(pkg_path)

			os.chdir(self.env.output_path)
			os.system('cp -f -r ')


	def _do_build(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.macOS or
			self.env.platform == Platform.iOS):
			if os.path.exists(self.env.output_lib_path+'/libSDL2.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print("    [{}] Start building ...".format(self.package_name))
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_path)
				os.system('{} ../configure --prefix={}; make -j {}; make install'.format(
					self.env.BUILD_FLAG,
					self.env.output_path,
					self.env.NJOBS
				))
				# os.system(DEBUG_BUILD_FLAG+'../configure --prefix='+output_path+';make -j'+NJOBS+';make install')
