#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform

class Builder_jsoncpp:
	def __init__(self):
		self.working_path = '.'
		self.platform = Platform.Windows
		self.env = None

		self.package_url = 'https://github.com/open-source-parsers/jsoncpp/archive/1.6.5.tar.gz'
		self.package_name = 'jsoncpp'
		self.archive_file = 'jsoncpp-1.6.5.tar.gz'

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
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.package_name
		)
		if(self.env.platform == Platform.Linux or
			self.env.platform == Platform.iOS):
			# Generate amalgamated source and header for jsoncpp
			if os.path.exists(build_path+'/dist'):
				print("    [{}] already built.".format(self.package_name))
			else:
				print('    [{}] Copying header files ...'.format(self.package_name))
				os.chdir(build_path)
				os.system('python amalgamate.py')
