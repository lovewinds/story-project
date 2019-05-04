#!/usr/bin/env python3
import os
from shutil import copytree, copy2, move
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pythonWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build Python3
        build_path = Path('{}/{}/PCbuild'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = f'{self.env.install_lib_path}\\{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        os.chdir(build_path)

        # Current version cannont detect the latest version of MSVC,
        # So just disable it
        copy2(f'{build_path}/env.bat',
              f'{build_path}/env_backup.bat')
        copy2(f'{self.env.patch_path}/python36_env.bat',
              f'{build_path}/env.bat')

        # Just build python core only
        BuildEnv.patch_static_MSVC("pythoncore.vcxproj", self.env.BUILD_TYPE)
        cmd = '''msbuild pcbuild.sln \
                    /maxcpucount:{} \
                    /t:pythoncore \
                    /p:PlatformToolSet={} \
                    /p:Configuration={} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # required only debug release
        if self.env.BUILD_TYPE == 'Debug':
            self.tag_log("Renaming built libraries ..")
            move(f'{self.env.install_lib_path}\\python36_d.lib',
                 f'{self.env.install_lib_path}\\python36.lib')
