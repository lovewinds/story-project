#!/usr/bin/env python3
import os
import glob
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

        # Patch all projects
        for proj in glob.glob(r'*.vcxproj'):
            self.tag_log(f'    Patching [{proj}]')
            BuildEnv.patch_static_MSVC(proj, self.env.BUILD_TYPE)
            # BuildEnv.patch_static_MSVC("pythoncore.vcxproj", self.env.BUILD_TYPE)
        BuildEnv.patch_static_props('pyproject.props', self.env.BUILD_TYPE)

        # Just build python core only
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

    def post(self):
        super().post()

        python_dir = Path('{}/{}'.format(
            self.env.source_path,
            self.config['name']
        ))
        python_header = Path(f'{python_dir}/Include')

        install_header_dir = f'{self.env.install_include_path}/python'

        # There is no header installation rule!
        if not os.path.exists(install_header_dir):
            try:
                copytree(python_header, install_header_dir)
                for h in glob.glob(f'{python_dir}/PC/*.h'):
                    copy2(h, install_header_dir)
                for h in glob.glob(f'{python_dir}/Python/*.h'):
                    copy2(h, install_header_dir)
            except FileExistsError:
                pass
        else:
            self.tag_log('Header files are already exists. Ignoring.')
