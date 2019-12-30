#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class pythonLinuxBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def build(self):
        build_path = '{}/{}/build'.format(
            self.env.source_path,
            self.config['name']
        )

        # if os.path.exists(self.env.install_lib_path+'/libpython3.6m.a'):
        _check = self.env.install_lib_path / self.config.get("checker")
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ...")
        BuildEnv.mkdir_p(build_path)
        os.chdir(build_path)
        cmd = '{} PATH={}:$PATH ../configure --prefix={}; make -j {}; make install'.format(
            self.env.BUILD_FLAG,
            self.env.install_bin_path,
            self.env.install_path,
            self.env.NJOBS
        )
        self.env.run_command(cmd, module_name=self.config['name'])

    def post(self):
        super().post()

        python_dir = Path('{}/{}'.format(
            self.env.source_path,
            self.config['name']
        ))

        # Create symbolic link
        os.chdir(self.env.install_include_path)
        if not os.path.isdir('python3.7'):
            cmd = 'ln -s python3.7m python3.7'
            self.env.run_command(cmd, module_name=self.config['name'])
        if not os.path.isdir('python'):
            cmd = 'ln -s python3.7m python'
            self.env.run_command(cmd, module_name=self.config['name'])

        os.chdir(self.env.install_bin_path)
        if not os.path.isfile('python'):
            cmd = 'ln -s python3 python'
            self.env.run_command(cmd, module_name=self.config['name'])

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
