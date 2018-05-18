#!/usr/bin/python
import os
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
				'build': None,
				'post': None,
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
