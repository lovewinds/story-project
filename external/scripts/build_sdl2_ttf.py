#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform

class Builder_SDL2_TTF:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz'
		self.package_name = 'SDL2_ttf'
		self.archive_file = 'SDL2_ttf-2.0.14.tar.gz'

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

		# Build FreeType
		print("  [#3] Building external lib [FreeType]")
		self._do_build_freetype()

	def _post_build(self):
		pass

	def _do_build(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.macOS or
			self.env.platform == Platform.iOS):
			if os.path.exists(self.env.output_lib_path+'/libSDL2_ttf.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_path)
				os.system('{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
					self.env.BUILD_FLAG,
					self.env.output_bin_path,
					self.env.output_path,
					self.env.NJOBS
				))

	def _do_build_freetype(self):
		build_path = '{}/{}/external/freetype-2.4.12/build'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.macOS or
			self.env.platform == Platform.iOS):
			if os.path.exists(self.env.output_bin_path+'/freetype-config'):
				print("    [FreeType2] already built.")
			else:
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_path)
				os.system('{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
					self.env.BUILD_FLAG,
					self.env.output_bin_path,
					self.env.output_path,
					self.env.NJOBS
				))
