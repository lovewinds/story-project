#!/usr/bin/python
import os
from shutil import copytree, copy2
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_python(Builder):
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
				'pre': None,
				'build': self._build_iOS,
				'post': None,
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

	def _pre_build_iOS(self):
		patch_python_apple(python_path)

	def _build_iOS(self):
		build_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.framework_path+'/Python.framework'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = 'make iOS'
		self.env.run_command(cmd, module_name=self.setup['name'])


# class Builder_python:
# 	def __init__(self):
# 		self.working_path = '.'
# 		self.platform = Platform.Windows
# 		self.env = None

# 		self.package_url = 'https://www.python.org/ftp/python/3.6.1/Python-3.6.1.tgz'
# 		self.package_name = 'python'
# 		self.archive_file = 'Python-3.6.1.tgz'

# 	def build(self, env_param):
# 		print("Building {} ...".format(self.package_name))
# 		self.env = env_param
# 		self._pre_build()
# 		self._do_build()
# 		self._post_build()

# 	def _pre_build(self):
# 		print("  [#0] Checking build output exists")

# 		print("  [#1] Downloading package")
# 		# Download process is not finished with content type issue.
# 		headers = {'Content-Type': 'application/x-gzip; charset=utf-8'}
# 		self.env.download_file(self.package_url, self.archive_file)

# 		print("  [#2] Extracting package")
# 		self.env.extract_tarball(self.archive_file, self.package_name)

# 		# Patch

# 	def _post_build(self):
# 		build_path = '{}/{}/build'.format(
# 			self.env.source_path,
# 			self.package_name
# 		)

# 		# There is no install rule, just copy library file into built directory.
# 		os.system('ln -s {}/python3.6m {}/python3.6'.format(
# 			self.env.output_include_path,
# 			self.env.output_include_path
# 		))
# 		os.system('ln -s {}/python3 {}/python'.format(
# 			self.env.output_bin_path,
# 			self.env.output_bin_path
# 		))

# 	def _do_build(self):
# 		build_parent_path = '{}/{}'.format(
# 			self.env.source_path,
# 			self.package_name
# 		)
# 		build_path = '{}/{}/build'.format(
# 			self.env.source_path,
# 			self.package_name
# 		)
# 		if(self.env.platform == Platform.Linux or
# 			self.env.platform == Platform.macOS or
# 			self.env.platform == Platform.iOS):
# 			if os.path.exists(self.env.output_lib_path+'/libpython3.6m.a'):
# 				print("    [{}] already built.".format(self.package_name))
# 			else:
# 				print("    [{}] Start building ...".format(self.package_name))
# 				BuildEnv.mkdir_p(build_path)
# 				os.chdir(build_parent_path)

# 				os.chdir(build_path)
# 				os.system('{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
# 					self.env.BUILD_FLAG,
# 					self.env.output_bin_path,
# 					self.env.output_path,
# 					self.env.NJOBS
# 				))
