#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class g3logLinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        self.patch_g3log_remove_warnings()

    def build(self):
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '''{} \
                cmake .. \
                    -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON \
                    -DCMAKE_BUILD_TYPE={} ; \
                make -j {} g3logger'''.format(
            self.env.BUILD_FLAG,
            self.env.BUILD_TYPE,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()

        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        # There is no install rule, just copy library file into built directory.
        if os.path.exists(f'{build_path}/libg3logger.a'):
                    copy2(f'{build_path}/libg3logger.a', self.env.install_lib_path)
        if os.path.exists(f'{build_path}/libg3logger.so'):
                    copy2(f'{build_path}/libg3logger.so', self.env.install_lib_path)
        if os.path.exists(f'{build_path}/libg3logger_shared.dylib'):
                    copy2(f'{build_path}/libg3logger_shared.dylib', self.env.install_lib_path)

    def patch_g3log_remove_warnings(self):
        patch_path = '{}/{}'.format(
            self.env.source_path,
            self.config['name']
        )
        os.chdir(patch_path)

        # Use patch script
        # https://github.com/techtonik/python-patch
        cmd = "{}/patch.py {}/g3log-1.2-remove_warnings.patch".format(
            self.env.working_path,
            self.env.patch_path
        )
        self.env.run_command(cmd, module_name=self.config['name'])

        self.tag_log("Patched")
