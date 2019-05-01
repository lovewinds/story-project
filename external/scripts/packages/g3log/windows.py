#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class g3logWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        super().build()

        # Build g3log
        build_path = Path('{}/{}/build'.format(
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
        # Disable fatal signal handling on g3log 1.3
        cmd = '''cmake .. \
                    -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON \
                    -DENABLE_FATAL_SIGNALHANDLING=OFF \
                    -DADD_BUILD_WIN_SHARED=ON \
                    -A x64'''
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # patch
        BuildEnv.patch_static_MSVC("g3logger.vcxproj", self.env.BUILD_TYPE)

        cmd = '''msbuild g3log.sln \
                    /maxcpucount:{} \
                    /t:g3logger \
                    /p:PlatformToolSet={} \
                    /p:Configuration={} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()
        
        patch_target = Path('{}/{}/src/g3log/g3log.hpp'.format(
            self.env.source_path,
            self.config['name']
        ))
        self.patch_file_encoding(patch_target)

    def patch_file_encoding(self, path):
        import codecs
        BLOCKSIZE = 1048576 # or some other, desired size in bytes

        from shutil import copyfile
        backup = f'{path}.old'
        if not os.path.exists(backup):
            copyfile(path, backup)

        with codecs.open(backup, "r", "utf-8") as sourceFile:
            with codecs.open(path, "w", "utf-16") as targetFile:
                while True:
                    contents = sourceFile.read(BLOCKSIZE)
                    if not contents:
                        break
                    targetFile.write(contents)
