#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform

class Builder(object):
	def __init__(self):
		self.env = None
		self.setup = {}

	def build(self, env_param):
		print("Building {} ...".format(self.setup['name']))
		self.env = env_param
		self.build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)

		self.__pre_build()
		self.__do_build()
		self.__post_build()

	def __pre_build(self):
		# print("  [#0] Check if build output exists")

		print("  [#1] Downloading package")
		package_url = None
		package_file = None
		try:
			if self.env.platform == Platform.Linux:
				package_url = self.setup['Linux']['url']
				package_file = self.setup['Linux']['filename']
			elif self.env.platform == Platform.macOS:
				package_url = self.setup['macOS']['url']
				package_file = self.setup['macOS']['filename']
			elif self.env.platform == Platform.iOS:
				package_url = self.setup['iOS']['url']
				package_file = self.setup['iOS']['filename']
		except:
			pass

		if package_url is None:
			package_url = self.setup['common']['url']
			package_file = self.setup['common']['filename']
		print("       URL: [{}]".format(package_url))
		self.env.download_file(package_url, package_file)

		print("  [#2] Extracting package")
		if package_file.endswith('.zip'):
			self.env.extract_zip(package_file, self.setup['name'])
		else:
			self.env.extract_tarball(package_file, self.setup['name'])

		# Execute pre build process
		platform_name = Platform.reverse_mapping[self.env.platform]
		platform = self.setup[platform_name]
		if platform is not None and platform['pre'] is not None:
			platform['pre']()

	def __post_build(self):
		# Implement for common post build processes here
		platform_name = Platform.reverse_mapping[self.env.platform]
		platform = self.setup[platform_name]
		if platform is not None and platform['post'] is not None:
			platform['post']()

	def __do_build(self):
		# Implement for common build processes here
		platform_name = Platform.reverse_mapping[self.env.platform]
		platform = self.setup[platform_name]
		if platform is not None and platform['build'] is not None:
			platform['build']()
