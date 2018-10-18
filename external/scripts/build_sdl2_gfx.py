#!/usr/bin/python
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.build_package import Builder

class Builder_SDL2_gfx(Builder):
	def __init__(self):
		super(Builder_SDL2_gfx, self)
		self.setup = {
			'name': 'SDL2_gfx',
			'common': {
				'url': 'http://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-1.0.4.tar.gz',
				'filename': 'SDL2_gfx-1.0.4.tar.gz'
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
				'build': self.build_iOS,
				'post': self.post_build_iOS,
			}
		}

	def _build_Linux(self):
		build_parent_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libSDL2_gfx.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_parent_path)
		cmd = '{} PATH={}:$PATH ./autogen.sh --prefix={}'.format(
			self.env.BUILD_FLAG,
			self.env.output_bin_path,
			self.env.output_path
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

	def build_iOS(self):
		build_parent_path = '{}/{}'.format(
			self.env.source_path,
			self.setup['name']
		)
		build_path = '{}/{}/build'.format(
			self.env.source_path,
			self.setup['name']
		)
		if os.path.exists(self.env.output_lib_path+'/libSDL2_gfx.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_parent_path)
		cmd = '{} PREFIX={} {}/ios-build.sh SDL2_gfx'.format(
			self.env.BUILD_FLAG,
			self.env.output_path,
			self.env.working_path
		)
		self.env.run_command(cmd, module_name=self.setup['name'])

	def post_build_iOS(self):
		# Copy header file
		headers = [
			'SDL2_framerate.h',
			'SDL2_rotozoom.h',
			'SDL2_imageFilter.h',
			'SDL2_gfxPrimitives.h',
			'SDL2_gfxPrimitives_font.h'
		]

		for header in headers:
			_header_source = '{}/{}/{}'.format(
				self.env.source_path,
				self.setup['name'],
				header
			)
			_header_dest = '{}/{}'.format(
				self.env.output_include_path,
				header
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
		headers = [
			'SDL2_framerate.h',
			'SDL2_rotozoom.h',
			'SDL2_imageFilter.h',
			'SDL2_gfxPrimitives.h',
			'SDL2_gfxPrimitives_font.h'
		]

		for header in headers:
			_header_src_file = '{}/{}'.format(_include_path, header)
			_header_dst_file = '{}/{}'.format(_framework_header_dir, header)
			copy2(_header_src_file, _header_dst_file)

		# Copy binaries
		print("       [{}] Framework : Copying binary  ...".format(self.setup['name']))
		BuildEnv.mkdir_p(_framework_dir)
		_lib_src_file = '{}/libSDL2_gfx.a'.format(self.env.output_lib_path)
		_lib_dst_file = '{}/SDL_gfx'.format(_framework_dir)
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
