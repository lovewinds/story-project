#!/usr/bin/env python3
import os
from shutil import copytree, copy2
from xml.etree import ElementTree
from pathlib import Path
from scripts.build_env import BuildEnv, Platform
from scripts.platform_builder import PlatformBuilder

class zeromqWindowsBuilder(PlatformBuilder):
    def __init__(self,
                 config_package: dict=None,
                 config_platform: dict=None):
        super().__init__(config_package, config_platform)

    def pre(self):
        super().pre()
        subpkg_url = 'https://github.com/zeromq/cppzmq/archive/v4.2.2.tar.gz'
        subpkg_name = 'cppzmq'
        subpkg_archive = 'cppzmq-4.2.2.tar.gz'

        self.tag_log("[CPPZMQ] Preparing sub package")
        self.env.download_file(subpkg_url, subpkg_archive)
        self.env.extract_tarball(subpkg_archive, subpkg_name)

    def build(self):
        super().build()

        # Build zeromq
        build_path = Path('{}/{}/builds/msvc/vs2015'.format(
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
        # Disable libsodium
        # TODO: Need to check for secure connection
        self.patch_libzmq_win(f"{build_path}/libzmq/libzmq.props")

        cmd = '''msbuild libzmq.sln \
                    /maxcpucount:{} \
                    /t:libzmq \
                    /p:Option-sodium=false \
                    /p:PlatformToolSet={} \
                    /p:Configuration=Static{} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

    def patch_libzmq_win(self, path):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        list = root.findall(msvc_ns_prefix+"PropertyGroup")
        for child in list:
            try:
                item = child.find(msvc_ns_prefix+"Linkage-libsodium")
                item.text = ""
            except:
                pass

        self.tag_log("Patched")

        tree.write(path, encoding="utf-8", xml_declaration=True)
