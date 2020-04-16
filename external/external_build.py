#!/usr/bin/env python3

import os
import sys
import math
import hashlib
import argparse
import traceback
import subprocess
import multiprocessing
import scripts.build_env as benv
from pathlib import Path
from scripts.build_env import BuildEnv, Platform

# TODO: Support dynamic import under specific folder
from scripts.packages.sdl import Builder_SDL2
from scripts.packages.libpng import Builder_libpng
from scripts.packages.zlib import Builder_zlib
from scripts.packages.bzip2 import Builder_bzip2
from scripts.packages.sdl_image import Builder_SDL2_Image
from scripts.packages.sdl_ttf import Builder_SDL2_TTF
from scripts.packages.sdl_gfx import Builder_SDL2_gfx
from scripts.packages.g3log import Builder_g3log
from scripts.packages.jsoncpp import Builder_jsoncpp
from scripts.packages.pugixml import Builder_pugixml
from scripts.packages.gtest import Builder_gtest
from scripts.packages.protobuf import Builder_protobuf
from scripts.packages.zeromq import Builder_zeromq
from scripts.packages.python3 import Builder_python
from scripts.packages.boost import Builder_boost
from scripts.packages.dirent import Builder_dirent
from scripts.packages.pybind11 import Builder_pybind11
from scripts.packages.icu import Builder_icu


# Start building
modules = [
	Builder_SDL2(),
	Builder_zlib(),
	Builder_bzip2(),
	Builder_libpng(),
	Builder_SDL2_Image(),
	Builder_SDL2_TTF(),
	Builder_g3log(),
	Builder_jsoncpp(),
	Builder_pugixml(),
	Builder_gtest(),
	Builder_protobuf(),
	Builder_pybind11(),
	Builder_python(),
	Builder_zeromq(),
	Builder_dirent(),
	Builder_icu(),
]

def check_msvc(env_param):
	found = True
	try:
		v = subprocess.run(['msbuild', '/ver'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		if v.returncode != 0:
			found = False
	except:
		found = False

	if not found:
		print(f'MSBuild is not found !')
		print('    Please set environment with script like below!')
		print('    "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\Tools\\VsDevCmd.bat"')

		exit(1)

def start_build(env_param):
	# Prepare environements
	BuildEnv.mkdir_p(env_param.install_bin_path)
	BuildEnv.mkdir_p(env_param.install_lib_path)
	BuildEnv.mkdir_p(env_param.install_include_path)
	BuildEnv.mkdir_p(env_param.install_packaging_path)

	# Build each modules
	for m in modules:
		result = False
		try:
			result = m.build(env_param)
		except Exception as e:
			print("Failed to build [{}] !".format(m.package.get('name')))
			traceback.print_exc(file=sys.stdout)
		
		if result == False:
			# TODO: Append into failed list and print detail log
			pass

def getHash(path, blocksize=65536):
	afile = open(path, 'rb')
	hasher = hashlib.md5()
	buf = afile.read(blocksize)
	while len(buf) > 0:
		hasher.update(buf)
		buf = afile.read(blocksize)
	afile.close()
	return hasher.hexdigest()

def update_cache(extdir):
	file_count = 0
	with open('script.cache', mode='w', encoding='utf-8') as cf:
		for folder, _, files in os.walk(extdir):
			rel_dir = os.path.relpath(Path(folder), extdir)
			if len(rel_dir) != '.' and not rel_dir.startswith('scripts'):
				continue
			for file in files:
				_, ext = os.path.splitext(file)
				if ext != '.py':
					continue
				archive_path = os.path.relpath(Path(folder) / file, extdir)
				hashstr = getHash(Path(folder) / file)
				cf.writelines(f'{hashstr} :: {archive_path}')
				file_count += 1
	list_hash = getHash('script.cache')
	print(f'Script cache :: [{list_hash}] / Total {file_count} scripts are found.\n')

if __name__ == "__main__":
	# Handle arguments
	parser = argparse.ArgumentParser(description='Build script for external packages !!! NOT supported yet!!!')
	parser.add_argument('--path', help='Set running path of script')
	parser.add_argument('--type', default='release',
					choices=['debug', 'release'],
					help='Select compile type (default: release)')
	parser.add_argument('--static', default='TRUE',
					choices=['TRUE', 'FALSE'],
					help='Select linking type (default: TRUE)')
	parser.add_argument('--arch', default='x64',
					choices=['x86', 'x64', 'x86_64'],
					help='Select archtecture for desktop build (default: x64)')
	parser.add_argument('--msvc', default='v141',
					help='If you use MSVC, you can select MSVC version to build with. (default: v141)')
	parser.add_argument('--platform', default='Windows',
					choices=['Windows', 'Linux', 'macOS', 'iOS'],
					help='Select platform to build. (default: Windows)')
	parser.add_argument('-v', '--verbose', action='store_true',
					help='Display compile messages (default: False)')
	parser.add_argument('--gen-cache', action='store_true',
					help='Only generates script cache list')
	args = parser.parse_args()

	# Set current working directory
	if not args.path:
		working_path = os.getcwd()
	else:
		working_path = args.path

	# Only generates cache
	if args.gen_cache:
		update_cache(working_path)
		exit(0)

	if args.platform == 'Windows':
		current_platform = Platform.Windows
	elif args.platform == 'Linux':
		current_platform = Platform.Linux
	elif args.platform == 'macOS':
		current_platform = Platform.macOS
	elif args.platform == 'iOS':
		current_platform = Platform.iOS
	else:
		current_platform = Platform.NotSupport

	env = BuildEnv(current_platform, args.type, working_path)
	env.verbose = args.verbose
	if current_platform == Platform.Windows:
		env.compiler_version = args.msvc

	print("###########################################")
	print("## Prepare to build external libraries ...")
	print("##")
	print("## Platform    : [ {} ]".format(
		Platform.name(current_platform)))
	if current_platform == Platform.Windows:
		print("##        MSVC : [ {} ]".format(args.msvc))
	print("## Build type  : [ {} ]".format(args.type))
	print("## Static link : [ {} ]".format(args.static))
	print("## Working path: [ {} ]".format(working_path))
	print("## NJOBS       : [ {} ]".format(env.NJOBS))
	print("##      (CPUs) : [ {} ]".format(multiprocessing.cpu_count()))
	print("###########################################")

	if current_platform == Platform.Windows:
		check_msvc(env)
	start_build(env)
