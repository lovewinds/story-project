#!/usr/bin/python
import os
from scripts.build_env import BuildEnv, Platform
from scripts.package_builder import PackageBuilder

class Builder_pugixml(PackageBuilder):
	def __init__(self):
		super(Builder_pugixml, self)
		self.setup = {
			'name': 'pugixml',
			'common': {
				'url': 'http://github.com/zeux/pugixml/releases/download/v1.7/pugixml-1.7.tar.gz',
				'filename': 'pugixml-1.7.tar.gz'
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
		build_path = '{}/{}/scripts/build'.format(
			self.env.source_path,
			self.setup['name']
		)

		if os.path.exists(self.env.output_lib_path+'/libpugixml.a'):
			print("       [{}] already built.".format(self.setup['name']))
			return

		print("       [{}] Start building ...".format(self.setup['name']))
		BuildEnv.mkdir_p(build_path)
		os.chdir(build_path)
		cmd = '{} cmake -DCMAKE_INSTALL_LIBDIR={} -DCMAKE_INSTALL_INCLUDEDIR={} ..; make -j {}; make install'.format(
			self.env.BUILD_FLAG,
			self.env.output_lib_path,
			self.env.output_include_path,
			self.env.NJOBS
		)
		self.env.run_command(cmd, module_name=self.setup['name'])
