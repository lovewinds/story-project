#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_zeromq(Builder):
	def __init__(self):
		super(Builder_zeromq, self)
		self.setup = {
			'name': 'zeromq',
			'common': {
				'url': 'https://github.com/zeromq/libzmq/releases/download/v4.2.1/zeromq-4.2.1.tar.gz',
				'filename': 'zeromq-4.2.1.tar.gz'
			},
			'Linux': {
				'pre': self._pre_build_Linux,
				'build': self._build_Linux,
				'post': None,
			},
			'macOS': {
				'pre': self._pre_build_Linux,
				'build': self._build_Linux,
				'post': None,
			},
			'iOS': {
				'pre': None,
				'build': None,
				'post': None,
			}
		}

	def _pre_build_Linux(self):
		subpkg_url = 'https://github.com/zeromq/cppzmq/archive/v4.2.2.tar.gz'
		subpkg_name = 'cppzmq'
		subpkg_archive = 'cppzmq-4.2.2.tar.gz'

		print("  Preparing sub package")
		self.env.download_file(subpkg_url, subpkg_archive)
		self.env.extract_tarball(subpkg_archive, subpkg_name)

		# Patch
		self._patch_libzmq_linux()

	def _patch_libzmq_linux(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		os.chdir('..')

		# Use patch script
		# https://github.com/techtonik/python-patch
		cmd = "{}/patch.py {}/libzmq.patch".format(
			self.env.working_path,
			self.env.patch_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
		os.chdir(build_path)
		print("       [{}] Patched".format(self.setup['name']))

	def _build_Linux(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libzmq-static.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX={} ; make -j {} libzmq-static; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
