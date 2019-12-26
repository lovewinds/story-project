#!/usr/bin/env python3
import os
import sys
import time
import math
import subprocess
import tarfile
import zipfile
import requests
import shutil
import multiprocessing
import errno
from shutil import move
from pathlib import Path
from xml.etree import ElementTree
from enum import Enum

# python 2.X
# def enum(*sequential, **named):
# 	try:
# 		enums = dict(zip(sequential, range(len(sequential))), **named)
# 		reverse = dict((value, key) for key, value in enums.iteritems())
# 		enums['reverse_mapping'] = reverse
# 	except AttributeError:
# 		enums = dict(zip(sequential, range(len(sequential))), **named)
# 		reverse = dict((value, key) for key, value in enums.items())
# 		enums['reverse_mapping'] = reverse
# 	return type('Enum', (), enums)

# Platform = enum('Windows', 'Linux', 'macOS', 'iOS', 'NotSupport')
# Platform = Enum('Windows', 'Linux', 'macOS', 'iOS', 'NotSupport')
class Platform(Enum):
    Windows = 0
    Linux = 1
    macOS = 2
    iOS = 3
    NotSupport = 4

    @classmethod
    def name(cls, name):
        if name in cls:
            return str(name).split('.')[-1]
        return 'NotSupport'

class BuildEnv:
    def __init__(self, platform, build_type, working_path, verbose=False):
        self.compiler_version = None
        self.verbose = verbose

        self.working_path = working_path
        self.platform = platform
        self.patch_path = Path('{}/patches'.format(self.working_path))
        self.temp_path = Path('{}/temp'.format(self.working_path))
        self.source_path = Path('{}/build/source/{}'.format(self.working_path,
                                    Platform.name(self.platform)))
        self.install_path = Path('{}/build/target/{}/{}'.format(self.working_path,
                                    Platform.name(self.platform), build_type))

        self.install_bin_path = Path('{}/bin'.format(self.install_path))
        self.install_lib_path = Path('{}/lib'.format(self.install_path))
        self.install_include_path = Path('{}/include'.format(self.install_path))
        self.install_packaging_path = Path('{}/packaging'.format(self.install_path))

        # For Apple framework plist creation
        self.framework_path = Path('{}/frameworks'.format(self.install_path))
        self.apple_framework_plist = '''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
    <dict>
        <key>CFBundleExecutable</key>
        <string>${FRAMEWORK_NAME}</string>
        <key>CFBundleIdentifier</key>
        <string>org.boost</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>6.0</string>
        <key>CFBundlePackageType</key>
        <string>FMWK</string>
        <key>CFBundleSignature</key>
        <string>????</string>
        <key>CFBundleVersion</key>
        <string>${FRAMEWORK_CURRENT_VERSION}</string>
    </dict>
</plist>
'''

        # Environments
        cpus = multiprocessing.cpu_count()
        self.NJOBS = str(int(math.ceil(cpus * 0.7)))
        # self.NJOBS = str(multiprocessing.cpu_count() / 2)
        if not self.NJOBS:
            self.NJOBS="1"

        # Currently, build process for external libs doesn't override main build flag.
        # It uses its separated build flags in order to build libs as release version.
        self.BUILD_FLAG = 'CFLAGS="-O2 -fPIC" CXXFLAGS="-O2 -fPIC" CPPFLAGS="-I{}" LDFLAGS="-L{}"'.format(
            self.install_include_path,
            self.install_lib_path
        )
        self.BUILD_TYPE = 'Release'
        if build_type == 'debug':
            self.BUILD_FLAG = 'CFLAGS="-g3 -O0 -fPIC" CXXFLAGS="-g3 -O0 -fPIC" CPPFLAGS="-I{}" LDFLAGS="-L{}"'.format(
                self.install_include_path,
                self.install_lib_path
            )
            self.BUILD_TYPE = 'Debug'

        # Use identical start time for all sub packages.
        now = time.localtime()
        self.log_time = '%04d%02d%02d_%02d%02d%02d' % (
                now.tm_year, now.tm_mon, now.tm_mday,
                now.tm_hour, now.tm_min, now.tm_sec)

    @staticmethod
    def mkdir_p(path):
        try:
            os.makedirs(path)
        except OSError as exc: # Python >2.5
            if exc.errno == errno.EEXIST and os.path.isdir(path):
                pass
            else: raise

    def download_file(self, url, file_name, headers=None):
        # Disable SSL verify to support proxy environment.
        self.mkdir_p(self.temp_path)
        if not os.path.isfile("{}/{}".format(self.temp_path, file_name)):
            # urllib.request.urlretrieve(url,
            # 	"{}/{}".format(self.temp_path, file_name))
            req = requests.get(url, stream=True, headers=headers, verify=False)
            if req.status_code == 200:
                with open("{}/{}".format(self.temp_path, file_name), 'wb') as f:
                    req.raw.decode_content = True
                    shutil.copyfileobj(req.raw, f)
            else:
                print("  status : {}".format(req.status_code))

            if not os.path.isfile("{}/{}".format(self.temp_path, file_name)):
                print("  Downloaded file not found! Please check internet connection, etc")
                raise FileNotFoundError

    def extract_zip(self, archive_file, package_name):
        if os.path.exists('{}/{}'.format(self.source_path, package_name)):
            print("       [{}] already extracted.".format(package_name))
        else:
            with zipfile.ZipFile('{}/{}'.format(self.temp_path, archive_file), 'r') as zf:
                zf.extractall(path='{}'.format(self.source_path))
            # Remove '.zip' from archive file name
            archive_name = archive_file.replace('.zip', '')
            move('{}/{}'.format(self.source_path, archive_name),
                 '{}/{}'.format(self.source_path, package_name))
            print("       [{}] extracted.".format(package_name))

    def extract_tarball(self, archive_file, package_name):
        if os.path.exists('{}/{}'.format(self.source_path, package_name)):
            print("       [{}] already extracted.".format(package_name))
        else:
            tarfile.open('{}/{}'.format(self.temp_path, archive_file)).extractall(self.source_path)
            # Remove '.tar.gz'
            archive_name = archive_file.replace('.tar.gz', '').replace('.tgz', '')
            move('{}/{}'.format(self.source_path, archive_name),
                 '{}/{}'.format(self.source_path, package_name))
            print("       [{}] extracted.".format(package_name))

    def run_command(self, cmd, module_name):
        str_out = ''
        log_file = ''
        if self.verbose:
            proc = subprocess.Popen(cmd, bufsize=-1, shell=True)
            while proc.poll() is None:
                time.sleep(1)
            (str_out, str_err) = proc.communicate(str.encode("utf-8"))
        else:
            # Store as a log file
            log_dir = '{}/logs/{}'.format(self.working_path, module_name)
            self.mkdir_p(log_dir)
            log_file = '{}/{}.{}.log'.format(log_dir, module_name, self.log_time)
            with open(log_file, 'a', encoding='utf-8') as f:
                f.write("="*80 + "\n")
                # f.write("\n")
                f.write("CMD : [{}]\n".format(cmd))
                f.write("="*80 + "\n")
                # f.write("\n")
                f.flush()
    
                proc = subprocess.Popen(cmd, bufsize=-1, shell=True,
                                    stdout=f, stderr=f)
                idx = 0
                while proc.poll() is None:
                    idx += 1
                    time.sleep(1)
                    s = "." * (idx % 10)
                    sys.stdout.write("\r       [{}]                         ".format(module_name))
                    sys.stdout.write("\r       [{}] {}".format(module_name, s))
                    sys.stdout.flush()

                sys.stdout.write("\r       [{}]                         ".format(module_name))
                sys.stdout.write("\r       [{}] Finished !\n".format(module_name))
                (str_out, str_err) = proc.communicate(str.encode("utf-8"))

            allow_codes = [0, 255]
            if proc.returncode not in allow_codes:
                # [TEST] Error handling
                print("")
                print("="*80)
                print("[ERROR] An error has been occured !")
                print("        Check following log file:")
                print("         {}".format(log_file))
                print("Return code: {}".format(proc.returncode))
                print("="*80)
                with open(log_file, 'r', encoding='utf-8') as f:
                    for line in f.readlines():
                        print(line.rstrip('\n'))

        return log_file

    @classmethod
    def patch_static_MSVC(cls, path, build_type='Release'):
        '''
        Change project configuration to build static library.
        :param path: Path for MSVC project file (*.vcxproj)
        :param type: Choose from ['Release', 'Debug']
        '''
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        # Change build result to .lib
        itemlist = root.findall(msvc_ns_prefix+"PropertyGroup")
        for child in itemlist:
            try:
                item = child.find(msvc_ns_prefix+"ConfigurationType")
                if item is not None and item.text == "DynamicLibrary":
                    item.text = "StaticLibrary"

                item = child.findall(msvc_ns_prefix+"TargetExt")
                for ext in item:
                    if ext.text == '.dll':
                        ext.text = '.lib'
            except:
                pass

        # Change runtime library
        itemlist = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in itemlist:
            try:
                # prune
                if child.attrib['Condition'].find(build_type) < 0:
                    continue

                item = child.find(msvc_ns_prefix+"ClCompile")
                item = item.find(msvc_ns_prefix+"RuntimeLibrary")

                # Change to proper build type
                if build_type.lower() == 'release':
                    if item.text == 'MultiThreadedDebugDLL':
                        item.text = 'MultiThreadedDLL'
                    if item.text == 'MultiThreadedDebug':
                        item.text = 'MultiThreaded'
                elif build_type.lower() == 'debug':
                    if item.text == 'MultiThreadedDLL':
                        item.text = 'MultiThreadedDebugDLL'
                    if item.text == 'MultiThreaded':
                        item.text = 'MultiThreadedDebug'
                
                # Change to /MD -> /MT (static)
                if item.text == 'MultiThreadedDLL':
                    item.text = "MultiThreaded"
                elif item.text == 'MultiThreadedDebugDLL':
                    item.text = "MultiThreadedDebug"
            except:
                pass

        print("        [MSVC] Patched")
        tree.write(path, encoding="utf-8", xml_declaration=True)

    @classmethod
    def patch_dynamic_MSVC(cls, path, build_type='Release'):
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        # Change build result to .lib
        itemlist = root.findall(msvc_ns_prefix+"PropertyGroup")
        for child in itemlist:
            try:
                item = child.find(msvc_ns_prefix+"ConfigurationType")
                if item is not None and item.text == "StaticLibrary":
                    item.text = "DynamicLibrary"

                item = child.findall(msvc_ns_prefix+"TargetExt")
                for ext in item:
                    if ext.text == '.lib':
                        ext.text = '.dll'
            except:
                pass

        # Change runtime library
        itemlist = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in itemlist:
            try:
                # prune
                if child.attrib['Condition'].find(build_type) < 0:
                    continue

                item = child.find(msvc_ns_prefix+"ClCompile")
                item = item.find(msvc_ns_prefix+"RuntimeLibrary")
                
                # Change to proper build type
                if build_type.lower() == 'release':
                    if item.text == 'MultiThreadedDebugDLL':
                        item.text = 'MultiThreadedDLL'
                    if item.text == 'MultiThreadedDebug':
                        item.text = 'MultiThreaded'
                elif build_type.lower() == 'debug':
                    if item.text == 'MultiThreadedDLL':
                        item.text = 'MultiThreadedDebugDLL'
                    if item.text == 'MultiThreaded':
                        item.text = 'MultiThreadedDebug'
                
                # Change to /MT -> /MD (dynamic)
                if item.text == 'MultiThreaded':
                    item.text = "MultiThreadedDLL"
                elif item.text == 'MultiThreadedDebug':
                    item.text = "MultiThreadedDebugDLL"
            except:
                pass

        print("        [MSVC] Patched")
        tree.write(path, encoding="utf-8", xml_declaration=True)

    @classmethod
    def patch_static_props(cls, path, build_type='Release'):
        '''
        Change project configuration to build static library.
        :param path: Path for MSVC property settings (*.props)
        :param type: Choose from ['Release', 'Debug']
        '''
        msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
        ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
        tree = ElementTree.parse(path)
        root = tree.getroot()

        # Change runtime library
        itemlist = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
        for child in itemlist:
            try:
                settings = child.findall(msvc_ns_prefix+"ClCompile")
                for comp in settings:
                    # prune
                    if (child.attrib.get('Condition') is not None
                            and child.attrib['Condition'].find(build_type) < 0):
                        continue

                    item = comp.find(msvc_ns_prefix+"RuntimeLibrary")

                    # Change to proper build type
                    if build_type.lower() == 'release':
                        if item.text == 'MultiThreadedDebugDLL':
                            item.text = 'MultiThreadedDLL'
                        if item.text == 'MultiThreadedDebug':
                            item.text = 'MultiThreaded'
                    elif build_type.lower() == 'debug':
                        if item.text == 'MultiThreadedDLL':
                            item.text = 'MultiThreadedDebugDLL'
                        if item.text == 'MultiThreaded':
                            item.text = 'MultiThreadedDebug'
                    
                    # Change to /MD -> /MT (static)
                    if item.text == 'MultiThreadedDLL':
                        item.text = "MultiThreaded"
                    elif item.text == 'MultiThreadedDebugDLL':
                        item.text = "MultiThreadedDebug"
            except:
                pass

        print("        [MSVC] Patched")
        tree.write(path, encoding="utf-8", xml_declaration=True)
