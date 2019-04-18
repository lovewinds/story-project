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
        super().__init__(config_package)

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]

    def pre(self):
        super().pre()

    def build(self):
        super().build()

        # Build g3log
        install_path = '{}/{}/release'.format(
            self.env.output_path,
            self.config['name']
        )
        build_path = '{}/{}/builds/msvc/vs2015'.format(
            self.env.source_path,
            self.config['name']
        )

        _check = f'{install_path}/{self.config.get("checker")}'
        if os.path.exists(_check):
            self.tag_log("Already built.")
            return

        self.tag_log("Start building ..")
        self.env.mkdir_p(build_path)
        os.chdir(build_path)
        # Disable libsodium
        # TODO: Need to check for secure connection
        self.patch_libzmq_win(f"{build_path}/libzmq/libzmq.props")

        # os.system('msbuild libzmq.sln /t:libzmq /p:Option-sodium=false /p:Configuration=DynRelease /p:PlatformToolSet='+MSVC_VER+' /p:OutDir='+build_path_rel)
        os.system('''
            msbuild libzmq.sln \
                /maxcpucount:{} \
                /t:libzmq \
                /p:Option-sodium=false \
                /p:PlatformToolSet={} \
                /p:Configuration=DynRelease \
                /p:Platform=x64 \
                /p:OutDir={}'''.format(
                    self.env.NJOBS, self.env.compiler_version, install_path))

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
