#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_jsoncpp(Builder):
	def __init__(self):
		super(Builder_jsoncpp, self)
		self.setup = {
			'name': 'jsoncpp',
			'common': {
				'url': 'https://github.com/open-source-parsers/jsoncpp/archive/1.6.5.tar.gz',
				'filename': 'jsoncpp-1.6.5.tar.gz'
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
				'pre': None,
				'build': None,
				'post': None,
			}
		}

	def _build_Linux(self):
		src_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)

		print("       [{}] Start building ...".format(self.setup['name']))
		if os.path.exists(src_path+'/dist'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		# Generate amalgamated source and header for jsoncpp
		print('       [{}] Copying header files ...'.format(self.setup['name']))
		os.chdir(src_path)
		cmd = 'python amalgamate.py'
		self.env.run_command(cmd, module_name=self.setup['name'])
