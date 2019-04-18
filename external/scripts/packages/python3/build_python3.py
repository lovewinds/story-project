#!/usr/bin/python
import os
import tarfile
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

class Builder_python(PackageBuilder):
	def __init__(self):
		super(Builder_python, self)
		self.setup = {
			'name': 'python',
			'common': {
				'url': 'https://www.python.org/ftp/python/3.6.1/Python-3.6.1.tgz',
				'filename': 'Python-3.6.1.tgz'
			},
			'Linux': {
				'pre': None,
				'build': self._build_Linux,
				'post': self._post_build,
			},
			'macOS': {
				'pre': None,
				'build': self._build_Linux,
				'post': self._post_build,
			},
			'iOS': {
				'url': 'https://github.com/pybee/Python-Apple-support/archive/3.6-b6.tar.gz',
				'filename': 'Python-Apple-support-3.6-b6.tar.gz',
				'pre': self._pre_build_iOS,
				'build': self._build_iOS,
				'post': self._post_build_iOS,
			}
		}

	def _post_build(self):
		# Create symbolic link
		os.chdir(self.env.output_include_path)
		if not os.path.exists('python3.6'):
			cmd = 'ln -s python3.6m python3.6'
			self.env.run_command(cmd, module_name=self.setup['name'])

		os.chdir(self.env.output_bin_path)
		if not os.path.exists('python'):
			cmd = 'ln -s python3 python'
			self.env.run_command(cmd, module_name=self.setup['name'])

	def _build_Linux(self):
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libpython3.6m.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def _patch_iphonesimulator(self):
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		print("       [{}-iOS] Patching python for iOS ...".format(self.setup['name']))
		os.chdir(build_path)
		# cmd = 'patch -p1 Makefile < {}/python_ios.patch'.format(
		# 	self.env.patch_path
		# )
		cmd = '{}/patch.py {}/python_ios.patch'.format(
			self.env.working_path,
			self.env.patch_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
		print("       [{}-iOS] Patched".format(self.setup['name']))

	def _custom_build_python_macOS(self):
		print("       [{}] Downloading python ...".format(self.setup['name']))
		package_url = self.setup['common']['url']
		package_file = self.setup['common']['filename']
		self.env.download_file(package_url, package_file)

		print("       [{}] Extracting ...".format(self.setup['name']))
		self.env.extract_tarball(package_file, 'python_bin')

		print("       [{}] Building python binary first ...".format(self.setup['name']))
		build_path = '{}/python_bin/build'.format(
			self.env.source_path
		)
		if os.path.exists(self.env.output_bin_path+'/python3'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

		# Create symlink (required to build boost)
		self._post_build()

	def _pre_build_iOS(self):
		self._patch_iphonesimulator()
		self._custom_build_python_macOS()

	def _build_iOS(self):
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.framework_path+'/Python.framework'):
			print("       [{}-iOS] already built.".format(self.setup['name']))
			return

		print("       [{}-iOS] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = 'make iOS'
		self.env.run_command(cmd, module_name=self.setup['name'])

	def _post_build_iOS(self):
		pkg_path = '{}/{}/dist'.format(
			self.env.source_path,
			self.setup['name']
		)
		path_framework = '{}/Python.framework'.format(
			self.env.framework_path,
		)

		if os.path.exists(self.env.framework_path+'/Python.framework'):
			print("       [{}-iOS] already installed.".format(self.setup['name']))
			return

		print("       [{}-iOS] Installing framework ...".format(self.setup['name']))
		BuildEnv.mkdir_p(self.env.framework_path)
		os.chdir(pkg_path)
		tarfile.open('Python-3.6-iOS-support.b6.tar.gz').extractall(path_framework)
		# os.rename('{}/Python-3.6-iOS-support.b6'.format(self.env.framework_path),
		# 		  '{}/Python.framework'.format(self.env.framework_path))
		os.chmod('{}/Support/Python/libPython.a'.format(path_framework), 0o755)
