#!/usr/bin/python

import os
import argparse
import multiprocessing
import scripts.build_env as benv

from scripts.build_env import BuildEnv, Platform

# TODO : Add build modules below
from scripts.build_sdl2 import Builder_SDL2
from scripts.build_sdl2_image import Builder_SDL2_Image
from scripts.build_sdl2_ttf import Builder_SDL2_TTF
from scripts.build_sdl2_gfx import Builder_SDL2_gfx
from scripts.build_g3log import Builder_g3log
from scripts.build_jsoncpp import Builder_jsoncpp
from scripts.build_pugixml import Builder_pugixml
from scripts.build_gtest import Builder_gtest
from scripts.build_protobuf import Builder_protobuf
from scripts.build_zeromq import Builder_zeromq
from scripts.build_python3 import Builder_python
from scripts.build_boost import Builder_boost

NJOBS = str(multiprocessing.cpu_count() / 2)
if not NJOBS:
	NJOBS="1"

# Start building
modules = [
	Builder_SDL2(),
	Builder_SDL2_Image(),
	Builder_SDL2_TTF(),
	Builder_SDL2_gfx(),
	Builder_g3log(),
	Builder_jsoncpp(),
	Builder_pugixml(),
	Builder_gtest(),
	Builder_protobuf(),
	Builder_zeromq(),
	Builder_python(),
	Builder_boost()
]

def start_build(env_param):
	for m in modules:
		m.build(env_param)

if __name__ == "__main__":
	# Handle arguments
	parser = argparse.ArgumentParser(description='Build script for external packages !!! NOT supported yet!!!')
	parser.add_argument('--path', help='Set running path of script')
	parser.add_argument('--type', default='debug',
					choices=['debug', 'release'],
					help='Select compile type (default: debug)')
	parser.add_argument('--static', default='TRUE',
					choices=['TRUE', 'FALSE'],
					help='Select linking type (default: TRUE)')
	parser.add_argument('--arch', default='x64',
					choices=['x86', 'x64', 'x86_64'],
					help='Select archtecture for desktop build (default: x86)')
	parser.add_argument('--msvc', default='v140',
					help='If you use MSVC, you can select MSVC version to build with. (default: v140)')
	parser.add_argument('--platform', default='Linux',
					choices=['Windows', 'Linux', 'macOS', 'iOS'],
					help='Select platform to build. (default: Windows)')
	parser.add_argument('--verbose', default='FALSE',
					choices=['TRUE', 'FALSE'],
					help='Display compile messages (default: FALSE)')
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

	print("###########################################")
	print("## Prepare to build external libraries ...")
	print("##")
	print("## Platform    : [ {} ]".format(
		Platform.reverse_mapping[CURRENT_PLATFORM]))
	if CURRENT_PLATFORM == Platform.Windows:
		print("##        MSVC : [ {} ]".format(args.msvc))
	print("## Build type  : [ {} ]".format(args.type))
	print("## Static link : [ {} ]".format(args.static))
	print("## Working path: [ {} ]".format(WORKING_PATH))
	print("## NJOBS       : [ {} ]".format(NJOBS))
	print("###########################################")

	start_build(env)
