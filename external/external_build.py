#!/usr/bin/python

import os
import sys
import math
import argparse
import traceback
import subprocess
import multiprocessing
import scripts.build_env as benv

from scripts.build_env import BuildEnv, Platform

# TODO: Support dynamic import under specific folder
from scripts.packages.sdl import Builder_SDL2
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

# Start building
modules = [
	# Builder_SDL2(),
	# Builder_SDL2_Image(),
	# Builder_SDL2_TTF(),
	# Builder_SDL2_gfx(),
	# Builder_g3log(),
	# Builder_jsoncpp(),
	# Builder_pugixml(),
	# Builder_gtest(),
	# Builder_protobuf(),
	# Builder_zeromq(),
	Builder_python(),
	# Builder_boost(),
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
		# "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"
		# "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
		print('    "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\VsDevCmd.bat"')
		print('    "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat"')

		exit(1)

def start_build(env_param):
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
	parser.add_argument('--msvc', default='v142',
					help='If you use MSVC, you can select MSVC version to build with. (default: v140)')
	parser.add_argument('--platform', default='Windows',
					choices=['Windows', 'Linux', 'macOS', 'iOS'],
					help='Select platform to build. (default: Windows)')
	parser.add_argument('-v', '--verbose', action='store_true',
					help='Display compile messages (default: False)')
	args = parser.parse_args()

	if args.platform == 'Windows':
		CURRENT_PLATFORM = Platform.Windows
	elif args.platform == 'Linux':
		CURRENT_PLATFORM = Platform.Linux
	elif args.platform == 'macOS':
		CURRENT_PLATFORM = Platform.macOS
	elif args.platform == 'iOS':
		CURRENT_PLATFORM = Platform.iOS
	else:
		CURRENT_PLATFORM = Platform.NotSupport

	# Set current working directory
	if not args.path:
		WORKING_PATH = os.getcwd()
	else:
		WORKING_PATH = args.path

	env = BuildEnv(CURRENT_PLATFORM, args.type, WORKING_PATH)
	env.verbose = args.verbose
	if CURRENT_PLATFORM == Platform.Windows:
		env.compiler_version = args.msvc

	print("###########################################")
	print("## Prepare to build external libraries ...")
	print("##")
	print("## Platform    : [ {} ]".format(
		Platform.name(CURRENT_PLATFORM)))
	if CURRENT_PLATFORM == Platform.Windows:
		print("##        MSVC : [ {} ]".format(args.msvc))
	print("## Build type  : [ {} ]".format(args.type))
	print("## Static link : [ {} ]".format(args.static))
	print("## Working path: [ {} ]".format(WORKING_PATH))
	print("## NJOBS       : [ {} ]".format(env.NJOBS))
	print("##      (CPUs) : [ {} ]".format(multiprocessing.cpu_count()))
	print("###########################################")

	if CURRENT_PLATFORM == Platform.Windows:
		check_msvc(env)
	start_build(env)
