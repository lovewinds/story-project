#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

class Builder_g3log(PackageBuilder):
	def __init__(self):
		super(Builder_g3log, self)
		self.setup = {
			'name': 'g3log',
			'common': {
				'url': 'https://github.com/KjellKod/g3log/archive/1.3.tar.gz',
				'filename': 'g3log-1.3.tar.gz'
			},
			'Linux': {
				'pre': self.patch_g3log_remove_warnings,
				'build': self._build_Linux,
				'post': self._post_build_Linux,
			},
			'macOS': {
				'pre': self.patch_g3log_remove_warnings,
				'build': self._build_Linux,
				'post': self._post_build_Linux,
			},
			'iOS': {
				'pre': None,
				'build': None,
				'post': None,
			}
		}

	def patch_g3log_remove_warnings(self):
		BuildEnv.mkdir_p(self.build_path)
		os.chdir(self.build_path)
		os.chdir('..')

		# Use patch script
		# https://github.com/techtonik/python-patch
		cmd = "{}/patch.py {}/g3log-1.2-remove_warnings.patch".format(
			self.env.working_path,
			self.env.patch_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

		os.chdir(self.build_path)
		print("       [{}] Patched".format(self.setup['name']))

	def _build_Linux(self):
		if os.path.exists(self.env.output_lib_path+'/libg3logger.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(self.build_path)
		os.chdir(self.build_path)
		cmd = '{} cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE={} ..; make -j {} g3logger; make -j {} g3logger_shared'.format(
			self.env.BUILD_FLAG,
			self.env.BUILD_CONF,
			self.env.NJOBS,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def _post_build_Linux(self):
		# There is no install rule, just copy library file into built directory.
		if os.path.exists('{}/libg3logger.a'.format(self.build_path)):
			copy2('{}/libg3logger.a'.format(self.build_path), self.env.output_lib_path)
		if os.path.exists('{}/libg3logger.so'.format(self.build_path)):
			copy2('{}/libg3logger.so'.format(self.build_path), self.env.output_lib_path)
		if os.path.exists('{}/libg3logger_shared.dylib'.format(self.build_path)):
			copy2('{}/libg3logger_shared.dylib'.format(self.build_path), self.env.output_lib_path)
