#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform

class Builder_pugixml:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'http://github.com/zeux/pugixml/releases/download/v1.7/pugixml-1.7.tar.gz'
		self.package_name = 'pugixml'
		self.archive_file = 'pugixml-1.7.tar.gz'

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
		pass

	def _do_build(self):
		build_path = '{}/{}/scripts/build'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.iOS):
			if os.path.exists(self.env.output_lib_path+'/libpugixml.a'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print("    [{}] Start building ...".format(self.package_name))
				BuildEnv.mkdir_p(build_path)
				os.chdir(build_path)
				os.system('{} cmake -DCMAKE_INSTALL_LIBDIR={} -DCMAKE_INSTALL_INCLUDEDIR={} ..; make -j {}; make install'.format(
					self.env.BUILD_FLAG,
					self.env.output_lib_path,
					self.env.output_include_path,
					self.env.NJOBS
				))
