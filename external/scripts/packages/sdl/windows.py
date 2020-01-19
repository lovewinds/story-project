#!/usr/bin/env python3
import os
from shutil import copytree, copy2, move
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class SDL2WindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build SDL2
        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        # Use default project configuration
        # os.chdir('{}/{}/VisualC'.format(
        #     self.env.source_path,
        #     self.config['name']))
        # self.env.patch_static_MSVC("SDL/SDL.vcxproj", self.env.BUILD_TYPE)
        # self.env.patch_static_MSVC("SDLmain/SDLmain.vcxproj", self.env.BUILD_TYPE)

        # Use CMake
        build_path = Path('{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        ))
        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '''cmake .. \
                    -A x64 \
                    -DFORCE_STATIC_VCRT=ON \
                    -DLIBC=ON'''
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        cmd = '''msbuild SDL2.sln \
                    /maxcpucount:{} \
                    /t:SDL2-static;SDL2main \
                    /p:PlatformToolSet={} \
                    /p:Configuration={} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # Copy headers
        self.tag_log("Copying header files ..")
        sdl2_include_path = self.env.source_path / self.config['name'] / 'include'
        _files = [x for x in sdl2_include_path.iterdir() if x.is_file()]
        for ff in _files:
            if not ff.name.endswith('.h'):
                continue
            copy2(str(ff), self.env.install_include_path)

        self.tag_log("Copying header files (platform customized) ..")
        copy2(build_path / 'include' / 'SDL_config.h',
              self.env.install_include_path / 'SDL_config.h')

        # required only debug release
        self.tag_log("Renaming built libraries ..")
        if self.env.BUILD_TYPE == 'Release':
            move(f'{self.env.install_lib_path}\\SDL2-static.lib',
                 f'{self.env.install_lib_path}\\SDL2.lib')
        elif self.env.BUILD_TYPE == 'Debug':
            move(f'{self.env.install_lib_path}\\SDL2maind.lib',
                 f'{self.env.install_lib_path}\\SDL2main.lib')
            move(f'{self.env.install_lib_path}\\SDL2-staticd.lib',
                 f'{self.env.install_lib_path}\\SDL2.lib')
