#!/usr/bin/python

import sys
import os
import commands
import errno
import tarfile
import platform
from shutil import copytree, copy2
from xml.etree import ElementTree
import argparse

WORKING_PATH = "./"
SDL2 = "SDL2-2.0.4"
SDL2_IMAGE = "SDL2_image-2.0.0"
SDL2_TTF = "SDL2_ttf-2.0.14"
SDL2_GFX = "SDL2_gfx-1.0.1"
JSONCPP = "jsoncpp-1.6.5"
G3LOG = "g3log-1.1"
PUGIXML = "pugixml-1.7"

def mkdir_p(path):
	try:
		os.makedirs(path)
	except OSError as exc: # Python >2.5
		if exc.errno == errno.EEXIST and os.path.isdir(path):
			pass
		else: raise

def patch_sdl2_image(path):
	msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
	ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
	tree = ElementTree.parse(path)
	root = tree.getroot()

	list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
	for child in list:
		item = child.find(msvc_ns_prefix+"ClCompile")
		item = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
		item.text = item.text.replace("..\\..\\SDL\\include", "..\\..\\..\\built\\include")

	list = root.findall(msvc_ns_prefix+"ItemGroup")
	print list
	for child in list:
		libPaths = child.findall(msvc_ns_prefix+"Library")
		if libPaths == None:
			continue
		for dir in libPaths:
			dir.attrib["Include"] = dir.attrib["Include"].replace("..\\..\\SDL\\VisualC\\SDL\\$(Platform)\\$(Configuration)\\SDL2.lib", "..\\..\\..\\built\\$(Configuration)\\SDL2.lib")
			dir.attrib["Include"] = dir.attrib["Include"].replace("..\\..\\SDL\\VisualC\\SDLmain\\$(Platform)\\$(Configuration)\\SDL2main.lib", "..\\..\\..\\built\\$(Configuration)\\SDL2main.lib")

	tree.write(path, encoding="utf-8", xml_declaration=True)
	print "Patched\n"

def patch_sdl2_ttf(path):
	msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
	ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
	tree = ElementTree.parse(path)
	root = tree.getroot()

	list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
	for child in list:
		item = child.find(msvc_ns_prefix+"ClCompile")
		item = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
		item.text = "..\\..\\..\\built\\include;"+item.text

		item = child.find(msvc_ns_prefix+"Link")
		item = item.find(msvc_ns_prefix+"AdditionalLibraryDirectories")
		item.text = "..\\..\\..\\built\\$(Configuration);"+item.text

	print "Patched\n"

	tree.write(path, encoding="utf-8", xml_declaration=True)

def patch_sdl2_gfx(path):
	msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
	ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
	tree = ElementTree.parse(path)
	root = tree.getroot()

	list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
	for child in list:
		item = child.find(msvc_ns_prefix+"ClCompile")
		item = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
		item.text = "..\\..\\built\\include;"+item.text

		item = child.find(msvc_ns_prefix+"Link")
		item = item.find(msvc_ns_prefix+"AdditionalLibraryDirectories")
		item.text = "..\\..\\built\\$(Configuration);"+item.text

	print "Patched\n"

	tree.write(path, encoding="utf-8", xml_declaration=True)

def extract_sources():
	print "Check source files ..."
	os.chdir(WORKING_PATH)

	# Extract sources
	source_path = WORKING_PATH+'sources/'
	mkdir_p(source_path)

	# extract SDL2
	if os.path.exists(source_path+'SDL2/'):
		print "SDL2 is already extracted."
	else:
		tarfile.open(SDL2+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2, source_path+'SDL2')

	# extract SDL2_image
	if os.path.exists(source_path+'SDL2_image/'):
		print "SDL2_image is already extracted."
	else:
		tarfile.open(SDL2_IMAGE+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2_IMAGE, source_path+'SDL2_image')

	# extract SDL2_ttf
	if os.path.exists(source_path+'SDL2_ttf/'):
		print "SDL2_ttf is already extracted."
	else:
		tarfile.open(SDL2_TTF+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2_TTF, source_path+'SDL2_ttf')

	# extract SDL2_gfx
	if os.path.exists(source_path+'SDL2_gfx/'):
		print "SDL2_gfx is already extracted."
	else:
		tarfile.open(SDL2_GFX+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2_GFX, source_path+'SDL2_gfx')

	# extract g3log
	if os.path.exists(source_path+'g3log/'):
		print "g3log is already extracted."
	else:
		tarfile.open(G3LOG+'.tar.gz').extractall(source_path)
		os.rename(source_path+G3LOG, source_path+'g3log')

	# extract jsoncpp
	if os.path.exists(source_path+'jsoncpp/'):
		print "jsoncpp is already extracted."
	else:
		tarfile.open(JSONCPP+'.tar.gz').extractall(source_path)
		os.rename(source_path+JSONCPP, source_path+'jsoncpp')

	# extract pugixml
	if os.path.exists(source_path+'pugixml/'):
		print "pugixml is already extracted."
	else:
		tarfile.open(PUGIXML+'.tar.gz').extractall(source_path)
		os.rename(source_path+PUGIXML, source_path+'pugixml')

def build_sources_MSVC(build_type):
	print "Trying to build extracted sources ..."
	build_path_dbg = WORKING_PATH+'built/Debug/'
	build_path_dbg = build_path_dbg.replace('/','\\')
	build_path_rel = WORKING_PATH+'built/Release/'
	build_path_rel = build_path_rel.replace('/','\\')
	include_path = WORKING_PATH+'built/include/'

	sdl2_path = WORKING_PATH+'sources/SDL2/VisualC/'
	sdl2_include_path = WORKING_PATH+'sources/SDL2/include/'
	sdl2_image_path = WORKING_PATH+'sources/SDL2_image/VisualC/'
	sdl2_ttf_path = WORKING_PATH+'sources/SDL2_ttf/VisualC/'
	sdl2_gfx_path = WORKING_PATH+'sources/SDL2_gfx/'
	g3log_path = WORKING_PATH+'sources/g3log/build/'
	jsoncpp_path = WORKING_PATH+'sources/jsoncpp/'

# Build SDL2
	if os.path.exists(build_path_rel+'SDL2.lib'):
		print "SDL2 is already built."
	else:
		os.chdir(sdl2_path)
		os.system('msbuild SDL.sln /t:SDL2;SDL2main /p:PlatformToolSet=v120 /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL.sln /t:SDL2;SDL2main /p:PlatformToolSet=v120 /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "Copying SDL2 header files .."
		copytree(sdl2_include_path, include_path)

# Build SDL2_image
	if os.path.exists(build_path_rel+'SDL2_image.lib'):
		print "SDL2_image is already built."
	else:
		os.chdir(sdl2_image_path)
		# !REQUIRED! Patch additional library and include path
		patch_sdl2_image("SDL_image_VS2012.vcxproj")
		os.system('msbuild SDL_image_VS2012.sln /t:SDL2_image /p:PlatformToolSet=v120 /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL_image_VS2012.sln /t:SDL2_image /p:PlatformToolSet=v120 /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "Copying SDL2_image header files .."
		copy2(sdl2_image_path+'../SDL_image.h', include_path+'SDL_image.h')
		# Copy external libraries
		# TODO: Fix hardcoded arch
		copy2(sdl2_image_path+'external/lib/x86/libjpeg-9.dll', build_path_dbg)
		copy2(sdl2_image_path+'external/lib/x86/libjpeg-9.dll', build_path_rel)
		copy2(sdl2_image_path+'external/lib/x86/libpng16-16.dll', build_path_dbg)
		copy2(sdl2_image_path+'external/lib/x86/libpng16-16.dll', build_path_rel)
		copy2(sdl2_image_path+'external/lib/x86/libtiff-5.dll', build_path_dbg)
		copy2(sdl2_image_path+'external/lib/x86/libtiff-5.dll', build_path_rel)
		copy2(sdl2_image_path+'external/lib/x86/libwebp-4.dll', build_path_dbg)
		copy2(sdl2_image_path+'external/lib/x86/libwebp-4.dll', build_path_rel)
		copy2(sdl2_image_path+'external/lib/x86/zlib1.dll', build_path_dbg)
		copy2(sdl2_image_path+'external/lib/x86/zlib1.dll', build_path_rel)

# Build SDL2_ttf
	if os.path.exists(build_path_rel+'SDL2_ttf.lib'):
		print "SDL2_ttf is already built."
	else:
		os.chdir(sdl2_ttf_path)
		# !REQUIRED! Patch additional library and include path
		patch_sdl2_ttf("SDL_ttf.vcxproj")
		os.system('msbuild SDL_ttf.sln /t:SDL2_ttf /p:PlatformToolSet=v120 /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL_ttf.sln /t:SDL2_ttf /p:PlatformToolSet=v120 /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "Copying SDL2_ttf header files .."
		copy2(sdl2_ttf_path+'../SDL_ttf.h', include_path+'SDL_ttf.h')
		# Copy external libraries
		# TODO: Fix hardcoded arch
		copy2(sdl2_ttf_path+'Win32/Debug/libfreetype-6.dll', build_path_dbg)
		copy2(sdl2_ttf_path+'Win32/Release/libfreetype-6.dll', build_path_rel)

# Build SDL2_gfx
	if os.path.exists(build_path_rel+'SDL2_gfx.lib'):
		print "SDL2_gfx is already built."
	else:
		os.chdir(sdl2_gfx_path)
		# !REQUIRED! Patch additional library and include path
		patch_sdl2_gfx("SDL2_gfx.vcxproj")
		os.system('msbuild SDL2_gfx.sln /t:SDL2_gfx /p:PlatformToolSet=v120 /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL2_gfx.sln /t:SDL2_gfx /p:PlatformToolSet=v120 /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "Copying SDL2_gfx header files .."
		copy2(sdl2_gfx_path+'SDL2_framerate.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_gfxPrimitives.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_gfxPrimitives_font.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_imageFilter.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_rotozoom.h', include_path)

# Build g3log
	if os.path.exists(build_path_rel+'g3logger.lib'):
		print "g3log is already built."
	else:
		print "Start building g3log .."
		mkdir_p(g3log_path)
		os.chdir(g3log_path)
		# TODO: Check 'CMAKE_BUILD_TYPE' is required if it builds both build type?
		#os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE='+BUILD_CONF+' -G "Visual Studio 12" ..')
		os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON ..')
		os.system('msbuild g3log.sln /t:g3logger /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild g3log.sln /t:g3logger /p:Configuration=Release /p:OutDir='+build_path_rel)

# Generate amalgamated source and header for jsoncpp
	if not os.path.exists(jsoncpp_path+'dist/'):
		os.chdir(jsoncpp_path)
		os.system('python amalgamate.py')

def build_sources(build_type):
	print "Trying to build extracted sources ..."
	build_path = WORKING_PATH+'built/'
	include_path = WORKING_PATH+'built/include/'

	sdl2_path = WORKING_PATH+'sources/SDL2/build/'
	sdl2_image_path = WORKING_PATH+'sources/SDL2_image/build/'
	sdl2_ttf_path = WORKING_PATH+'sources/SDL2_ttf/build/'
	sdl2_gfx_path = WORKING_PATH+'sources/SDL2_gfx/build/'
	g3log_path = WORKING_PATH+'sources/g3log/build/'
	jsoncpp_path = WORKING_PATH+'sources/jsoncpp/'

	# TODO: Check its working
	if build_type == 'debug':
		DEBUG_BUILD_FLAG = 'CFLAGS="-ggdb3 -O0" CXXFLAGS="-ggdb3 -O0" LDFLAGS="-ggdb3" '
		BUILD_CONF = 'Debug'
	else:
		DEBUG_BUILD_FLAG = ''
		BUILD_CONF = 'Release'
	mkdir_p(build_path)

# Build SDL2
	if os.path.exists(build_path+'lib/libSDL2.a'):
		print "SDL2 is already built."
	else:
		mkdir_p(sdl2_path)
		os.chdir(sdl2_path)
		os.system(DEBUG_BUILD_FLAG+'../configure --prefix='+build_path+';make;make install')

# Build SDL2_image
	if os.path.exists(build_path+'lib/libSDL2_image.a'):
		print "SDL2_image is already built."
	else:
		mkdir_p(sdl2_image_path)
		os.chdir(sdl2_image_path)
		print '*********************'
		os.system(DEBUG_BUILD_FLAG+'../configure --prefix='+build_path+';make;make install')

# Build SDL2_ttf
	if os.path.exists(build_path+'lib/libSDL2_ttf.a'):
		print "SDL2_ttf is already built."
	else:
		mkdir_p(sdl2_ttf_path)
		os.chdir(sdl2_ttf_path)
		os.system(DEBUG_BUILD_FLAG+'../configure --prefix='+build_path+';make;make install')

# Build SDL2_gfx
	if os.path.exists(build_path+'lib/libSDL2_gfx.a'):
		print "SDL2_gfx is already built."
	else:
		mkdir_p(sdl2_gfx_path)
		os.chdir(sdl2_gfx_path)
		os.system(DEBUG_BUILD_FLAG+'../configure --prefix='+build_path+';make;make install')

# Build g3log
	if os.path.exists(build_path+'libg3logger.a'):
		print "g3log is already built."
	else:
		print "Start building g3log .."
		mkdir_p(g3log_path)
		os.chdir(g3log_path)
		os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE='+BUILD_CONF+' ..; make g3logger')
		# There is no install rule, just copy library file into built directory.
		copy2(g3log_path+'libg3logger.a', build_path+'lib/')

# Generate amalgamated source and header for jsoncpp
	if not os.path.exists(jsoncpp_path+'dist/'):
		os.chdir(jsoncpp_path)
		os.system('python amalgamate.py')

if __name__ == "__main__":
	# Handle arguments
	parser = argparse.ArgumentParser(description='Build script for external packages !!!NOT supported yet!!!')
	parser.add_argument('--path', help='Set running path of script')
	parser.add_argument('--type', default='release', choices=['debug', 'release'], help='Select compile type (default: release)')
	parser.add_argument('--arch', default='x86', choices=['x86', 'x64', 'x86_64'], help='Select archtecture for desktop build (default: x86)')
	parser.add_argument('--vc-version', default='vc120', help='If you use MSVC, you can select MSVC version to build with. (default: vc120)')
	args = parser.parse_args()

	print "Build type : "+args.type

	# Set current working directory
	if not args.path:
		WORKING_PATH = os.getcwd() + '/'
	else:
		WORKING_PATH = args.path + '/'
	print "Script working on: ["+WORKING_PATH+"]"

	# Extract sources from archive files
	extract_sources()

	# Build sources
	if platform.system().startswith('Linux'):
		print 'Linux'
		build_sources(args.type)
	elif platform.system() == 'Windows':
		print 'Windows'
		WORKING_PATH = WORKING_PATH.replace('\\', '/')
		build_sources_MSVC(args.type)
	elif platform.system() == 'Darwin':
		print 'MAC OS'
		build_sources(args.type)
	else:
		print 'Current OS is not supported yet'
