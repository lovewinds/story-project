#!/usr/bin/python
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_SDL2_Image(Builder):
	def __init__(self):
		super(Builder_SDL2_Image, self)
		self.setup = {
			'name': 'SDL2_image',
			'common': {
				'url': 'https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.1.tar.gz',
				'filename': 'SDL2_image-2.0.1.tar.gz'
			},
			'Linux': {
				'pre': self.pre_build,
				'build': self._build_Linux,
				'post': None,
			},
			'macOS': {
				'pre': self.pre_build,
				'build': self._build_Linux,
				'post': None,
			},
			'iOS': {
				'pre': None,
				'build': self.build_iOS,
				'post': self.post_build_iOS,
			}
		}

	def _build_Linux(self):
		pkg_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libSDL2_image.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(pkg_path)
		cmd = './autogen.sh --prefix={}'.format(
			self.env.output_bin_path,
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def pre_build(self):
		self.ext_build_png()
		self.ext_build_jpeg()

	def ext_build_jpeg(self):
		ext_path = '{}/{}/external'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/external/jpeg-9/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libjpeg.a'):
			print("       [{}] already built.".format('libjpeg'))
			return

		print("       [{}] Start building ...".format('libjpeg'))
		# Apply patch
		os.chdir(ext_path)
		cmd = "{}/patch.py jpeg-9.patch".format(
			self.env.working_path
		)
		self.env.run_command(cmd, module_name='libjpeg')

		# Build
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name='libjpeg')

	def ext_build_png(self):
		ext_path = '{}/{}/external'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/external/libpng-1.6.2/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libpng16.a'):
			print("       [{}] already built.".format('libpng'))
			return

		print("       [{}] Start building ...".format('libpng'))
		# Apply patch
		os.chdir(ext_path)
		cmd = "{}/patch.py libpng-1.6.2.patch".format(
			self.env.working_path
		)
		self.env.run_command(cmd, module_name='libpng')

		# Build
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name='libpng')

	def build_iOS(self):
		build_path = '{}/{}/Xcode-iOS'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libSDL2_image.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} PREFIX={} {}/ios-build.sh SDL2_image'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
			self.env.working_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def post_build_iOS(self):
		# Copy header file
		_header_source = '{}/{}/SDL_image.h'.format(
			self.env.source_path,
			self.setup['name']
		)
		_header_dest = '{}/SDL_image.h'.format(
			self.env.output_include_path
		)
		print("       [{}] copying header ...".format(self.setup['name']))
		copy2(_header_source, _header_dest)

		self.create_framework_iOS()

	def create_framework_iOS(self):
		# Required path
		_include_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		_framework_dir = '{}/{}.framework'.format(
			self.env.framework_path,
			self.setup['name'],
		)
		_framework_header_dir = '{}/{}.framework/Headers'.format(
			self.env.framework_path,
			self.setup['name'],
		)
		_framework_resource_dir = '{}/{}.framework/Resources'.format(
			self.env.framework_path,
			self.setup['name'],
		)

		# Copy headers into Framework directory
		print("       [{}] Framework : Copying header ...".format(self.setup['name']))
		BuildEnv.mkdir_p(_framework_header_dir)
		_header_src_file = '{}/SDL_image.h'.format(_include_path)
		_header_dst_file = '{}/SDL_image.h'.format(_framework_header_dir)
		copy2(_header_src_file, _header_dst_file)

		# Copy binaries
		print("       [{}] Framework : Copying binary  ...".format(self.setup['name']))
		BuildEnv.mkdir_p(_framework_dir)
		_lib_src_file = '{}/libSDL2_image.a'.format(self.env.output_lib_path)
		_lib_dst_file = '{}/SDL_image'.format(_framework_dir)
		copy2(_lib_src_file, _lib_dst_file)

		# Create plist
		BuildEnv.mkdir_p(_framework_resource_dir)
		plist_str = self.env.apple_framework_plist.replace(
			'${FRAMEWORK_NAME}', self.setup['name']
		).replace(
			'${FRAMEWORK_CURRENT_VERSION}', self.setup['name']
		)
		plist_file = '{}/Info.plist'.format(_framework_resource_dir)
		with open(plist_file, "w") as pf:
			pf.write(plist_str)
