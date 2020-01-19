#!/usr/bin/env python3
import os
import glob
import subprocess
from shutil import copytree, copy2, move
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pythonWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()

        # Prepare external libraries (including BZip2)
        os.chdir(self.env.source_path / self.config['name'] / 'PCBuild')
        cmd = 'get_externals.bat'
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])


    def build(self):
        super().build()

        # Build Python3
        build_path = Path('{}/{}/PCbuild'.format(
            self.env.source_path,
            self.config['name']
        ))

        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        os.chdir(build_path)

        # Patch all projects
        # BuildEnv.patch_static_MSVC("pythoncore.vcxproj", self.env.BUILD_TYPE)
        
        try:
            winsdk = subprocess.check_output('reg query "HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0" /v ProductVersion', shell=True)
            winsdk = str(winsdk).split('\\r\\n')[2]
            winsdk = winsdk.strip().split()[2]
        except:
            winsdk = 'N/A'
        if winsdk == 'N/A':
            try:
                winsdk = subprocess.check_output('reg query "HKLM\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v10.0" /v ProductVersion', shell=True)
                winsdk = str(winsdk).split('\\r\\n')[2]
                winsdk = winsdk.strip().split()[2]
            except:
                winsdk = '10.0'
        # WinSDK version string format should be '10.0.XXXXX.0'
        if winsdk.count('.') == 2:
            winsdk += '.0'
        self.tag_log(f'Windows SDK :: {winsdk}')

        for proj in glob.glob(r'*.vcxproj'):
            self.tag_log(f'    Patching [{proj}]')
            BuildEnv.patch_static_MSVC(proj, self.env.BUILD_TYPE)
            BuildEnv.patch_SDK_version(proj, winsdk)
        BuildEnv.patch_static_props('pyproject.props', self.env.BUILD_TYPE)

        # Just build python core only
        cmd = '''msbuild pcbuild.sln \
                    /maxcpucount:{} \
                    /t:_bz2,pythoncore \
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
            if os.path.exists(self.env.install_lib_path / 'python37_d.lib'):
                self.tag_log("Renaming built libraries ..")
                move(f'{self.env.install_lib_path}\\python37_d.lib',
                    f'{self.env.install_lib_path}\\python37.lib')

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

        # Process custom module installation
        import zipfile
        pkgdirs = ['Lib', 'Tools']
        try:
            for pkgdir in pkgdirs:
                index = 0
                module_file = self.env.install_path / f'python37_modules_{pkgdir}.zip'
                if os.path.exists(module_file):
                    self.tag_log('Uses already created module files')
                    return
                with zipfile.ZipFile(module_file, 'w') as zf:
                    # os.chdir(python_dir / pkgdir)
                    # for fileitem in os.listdir(python_dir / pkgdir):
                    #     # archive_path = os.path.relpath(fileitem, python_dir / pkgdir)
                    #     zf.write(fileitem, compress_type = zipfile.ZIP_DEFLATED)

                    #     print(f'{index:03d} :: {fileitem}')
                    #     index += 1

                    for folder, subdir, files in os.walk(python_dir / pkgdir):
                        for file in files:
                            archive_path = os.path.relpath(Path(folder) / file, python_dir / pkgdir)
                            zf.write(
                                Path(folder) / file,
                                archive_path,
                                compress_type = zipfile.ZIP_DEFLATED)
                            index += 1
                self.tag_log(f'{index:03d} files archived')
        except FileExistsError:
            self.tag_log('Uses already created module files')
