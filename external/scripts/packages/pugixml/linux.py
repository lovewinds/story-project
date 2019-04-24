#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pugixmlLinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/scripts/build'.format(
            self.env.source_path,
            self.config['name']
        )

        # if os.path.exists(self.env.output_lib_path+'/libpugixml.a'):
        _check = f'{self.env.output_lib_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} cmake -DCMAKE_INSTALL_LIBDIR={} -DCMAKE_INSTALL_INCLUDEDIR={} ..; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.output_lib_path,
            self.env.output_include_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])
