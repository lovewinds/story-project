#!/usr/bin/env python3
import os
import glob
from shutil import copytree, copy2, move
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
        subpkg_url = 'https://github.com/zeromq/cppzmq/archive/v4.3.0.tar.gz'
        subpkg_name = 'cppzmq'
        subpkg_archive = 'cppzmq-4.3.0.tar.gz'

        self.tag_log("[CPPZMQ] Preparing sub package")
        self.env.download_file(subpkg_url, subpkg_archive)
        self.env.extract_tarball(subpkg_archive, subpkg_name)

    def build(self):
        super().build()

        self.build_libzmq()

    def post(self):
        super().post()

        if self.env.BUILD_TYPE == 'Debug':
            self.tag_log("Renaming built libraries ..")
            if os.path.exists(Path(f'{self.env.install_lib_path}\\libzmq-v141-mt-sgd-4_3_1.lib')):
                move(f'{self.env.install_lib_path}\\libzmq-v141-mt-sgd-4_3_1.lib',
                    f'{self.env.install_lib_path}\\libzmq.lib')

    def build_libzmq(self):
        # Build zeromq
        # build_path = Path('{}/{}/builds/msvc/vs2015'.format(
        build_path = Path('{}/{}/build'.format(
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

        # CMake build
        cmd = '''cmake .. -A x64 \
                    -D POLLER="" \
                    -D WITH_LIBSODIUM=OFF \
                    -DWITH_PERF_TOOL=OFF \
                    -DZMQ_BUILD_TESTS=OFF \
                    -DCMAKE_INSTALL_PREFIX={} \
                    '''.format(
                        self.env.install_path
                    )
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name='cppzmq')


        BuildEnv.patch_static_MSVC(Path(f'{build_path}/libzmq-static.vcxproj'), self.env.BUILD_TYPE)
        cmd = '''msbuild ZeroMQ.sln \
                    /maxcpucount:{} \
                    /t:libzmq-static \
                    /p:Option-sodium=false \
                    /p:PlatformToolSet={} \
                    /p:Configuration={} \
                    /p:Platform=x64 \
                    /p:OutDir={}\\ \
                '''.format(self.env.NJOBS,
                           self.env.compiler_version, self.env.BUILD_TYPE,
                           self.env.install_lib_path)
        # TODO: Apply CMake installation
        # cmd = '''cmake --build . \
        #             -j {} \
        #             --config {} \
        #             --target install \
        #             '''.format(
        #                 self.env.NJOBS,
        #                 self.env.BUILD_TYPE
        #             )
        self.log('\n          '.join(f'    [CMD]:: {cmd}'.split()))
        self.env.run_command(cmd, module_name=self.config['name'])

        # Rename to 'libzmq.lib'
        os.chdir(self.env.install_lib_path)
        for proj in glob.glob(r'libzmq*.lib'):
            self.tag_log(f'    Patching [{proj}]')
            os.renames(proj, 'libzmq.lib')

    def patch_libzmq_prop(self, path):
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
