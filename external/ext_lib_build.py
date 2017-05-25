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
import multiprocessing

CURRENT_PLATFORM = 0
WORKING_PATH = "./"
SDL2 = "SDL2-2.0.5"
SDL2_IMAGE = "SDL2_image-2.0.1"
SDL2_TTF = "SDL2_ttf-2.0.14"
SDL2_GFX = "SDL2_gfx-1.0.3"
JSONCPP = "jsoncpp-1.6.5"
G3LOG = "g3log-1.2"
PUGIXML = "pugixml-1.7"
GTEST = "googletest-release-1.7.0"
PROTOBUF = "protobuf-3.0.0"
ZEROMQ = "zeromq-4.2.1"
CPPZMQ = "cppzmq"
MSVC_VER = "v140"
STATIC_LINK = False
NJOBS = str(multiprocessing.cpu_count())
if not NJOBS:
	NJOBS="1"

def enum(*sequential, **named):
	enums = dict(zip(sequential, range(len(sequential))), **named)
	reverse = dict((value, key) for key, value in enums.iteritems())
	enums['reverse_mapping'] = reverse
	return type('Enum', (), enums)

def mkdir_p(path):
	try:
		os.makedirs(path)
	except OSError as exc: # Python >2.5
		if exc.errno == errno.EEXIST and os.path.isdir(path):
			pass
		else: raise

def patch_static_MSVC(path):
	msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
	ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
	tree = ElementTree.parse(path)
	root = tree.getroot()

	# Change build result to .lib
	list = root.findall(msvc_ns_prefix+"PropertyGroup")
	for child in list:
		try:
			item = child.find(msvc_ns_prefix+"ConfigurationType")
			if item.text == "DynamicLibrary":
				item.text = "StaticLibrary"
		except:
			pass

	# Change runtime library
	list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
	for child in list:
		try:
			item = child.find(msvc_ns_prefix+"ClCompile")
			item = item.find(msvc_ns_prefix+"RuntimeLibrary")
			if item.text == 'MultiThreadedDLL':
				item.text = "MultiThreaded"
			elif item.text == 'MultiThreadedDebugDLL':
				item.text = "MultiThreadedDebug"
		except:
			pass

	print "   [SDL2] Patched\n"

	tree.write(path, encoding="utf-8", xml_declaration=True)

def patch_sdl2_image(path):
	msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
	ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
	tree = ElementTree.parse(path)
	root = tree.getroot()

	list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
	for child in list:
		item = child.find(msvc_ns_prefix+"ClCompile")
		item_include = item.find(msvc_ns_prefix+"AdditionalIncludeDirectories")
		item_include.text = "..\\..\\..\\built\\include;"+item_include.text

		item = child.find(msvc_ns_prefix+"Link")
		item_lib = item.find(msvc_ns_prefix+"AdditionalLibraryDirectories")
		if item_lib == None:
			note = ElementTree.Element(msvc_ns_prefix+"AdditionalLibraryDirectories")
			note.text = "..\\..\\..\\built\\$(Configuration);"
			item.append(note)

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
	print "   [SDL2_image] Patched\n"

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

	print "   [SDL2_ttf] Patched\n"

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

	list = root.findall(msvc_ns_prefix+"PropertyGroup")
	for child in list:
		item = child.find(msvc_ns_prefix+"WindowsTargetPlatformVersion")
		if item != None:
			child.remove(item)

	print "   [SDL2_gfx] Patched\n"

	tree.write(path, encoding="utf-8", xml_declaration=True)

def patch_gtest(path):
	msvc_ns_prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
	ElementTree.register_namespace('', "http://schemas.microsoft.com/developer/msbuild/2003")
	tree = ElementTree.parse(path)
	root = tree.getroot()

	list = root.findall(msvc_ns_prefix+"ItemDefinitionGroup")
	for child in list:
		item = child.find(msvc_ns_prefix+"ClCompile")
		item = item.find(msvc_ns_prefix+"RuntimeLibrary")
		if item.text == 'MultiThreaded':
			item.text = "MultiThreadedDLL"
		elif item.text == 'MultiThreadedDebug':
			item.text = "MultiThreadedDebugDLL"

	print "   [gtest] Patched\n"

	tree.write(path, encoding="utf-8", xml_declaration=True)

def patch_libzmq_linux(path):
	os.chdir(path)
	os.chdir('..')
	print "Current path: ["+os.getcwd()+"]"
	os.system('patch -p0 < ../../libzmq.patch')
	os.chdir(path)
	print "   [ZeroMQ] Patched\n"

def patch_g3log_remove_warnings(path):
	os.chdir(path)
	os.chdir('..')
	print "Current path: ["+os.getcwd()+"]"
	# Use patch script
	# https://github.com/techtonik/python-patch
	os.system('../../patch.py ../../g3log-1.2-remove_warnings.patch')
	os.chdir(path)
	print "   [g3log] Patched\n"

def patch_libzmq_win(path):
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

	print "  [ZeroMQ] Patched\n"

	tree.write(path, encoding="utf-8", xml_declaration=True)

def extract_sources():
	print "\n"
	print "#########################################"
	print "## Checking source files ..."
	print "#########################################"
	os.chdir(WORKING_PATH)

	# Extract sources
	source_path = WORKING_PATH+'sources/'
	mkdir_p(source_path)

	# extract SDL2
	if os.path.exists(source_path+'SDL2/'):
		print "   [SDL2] already extracted."
	else:
		tarfile.open(SDL2+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2, source_path+'SDL2')
		print "   [SDL2] extracted."

	# extract SDL2_image
	if os.path.exists(source_path+'SDL2_image/'):
		print "   [SDL2_image] already extracted."
	else:
		tarfile.open(SDL2_IMAGE+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2_IMAGE, source_path+'SDL2_image')
		print "   [SDL2_image] extracted."

	# extract SDL2_ttf
	if os.path.exists(source_path+'SDL2_ttf/'):
		print "   [SDL2_ttf] already extracted."
	else:
		tarfile.open(SDL2_TTF+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2_TTF, source_path+'SDL2_ttf')
		print "   [SDL2_ttf] extracted."

	# extract SDL2_gfx
	if os.path.exists(source_path+'SDL2_gfx/'):
		print "   [SDL2_gfx] already extracted."
	else:
		tarfile.open(SDL2_GFX+'.tar.gz').extractall(source_path)
		os.rename(source_path+SDL2_GFX, source_path+'SDL2_gfx')
		print "   [SDL2_gfx] extracted."

	# extract g3log
	if os.path.exists(source_path+'g3log/'):
		print "   [g3log] already extracted."
	else:
		tarfile.open(G3LOG+'.tar.gz').extractall(source_path)
		os.rename(source_path+G3LOG, source_path+'g3log')
		print "   [g3log] extracted."

	# extract jsoncpp
	if os.path.exists(source_path+'jsoncpp/'):
		print "   [jsoncpp] already extracted."
	else:
		tarfile.open(JSONCPP+'.tar.gz').extractall(source_path)
		os.rename(source_path+JSONCPP, source_path+'jsoncpp')
		print "   [jsoncpp] extracted."

	# extract pugixml
	if os.path.exists(source_path+'pugixml/'):
		print "   [pugixml] already extracted."
	else:
		tarfile.open(PUGIXML+'.tar.gz').extractall(source_path)
		os.rename(source_path+PUGIXML, source_path+'pugixml')
		print "   [pugixml] extracted."

	# extract gtest
	if os.path.exists(source_path+'gtest/'):
		print "   [gtest] already extracted."
	else:
		tarfile.open(GTEST+'.tar.gz').extractall(source_path)
		os.rename(source_path+GTEST, source_path+'gtest')
		print "   [gtest] extracted."

	# extract protobuf
	if os.path.exists(source_path+'protobuf/'):
		print "   [protobuf] already extracted."
	else:
		tarfile.open(PROTOBUF+'.tar.gz').extractall(source_path)
		os.rename(source_path+PROTOBUF, source_path+'protobuf')
		print "   [protobuf] extracted."

	# extract ZeroMQ
	if os.path.exists(source_path+'zeromq/'):
		print "   [ZeroMQ] already extracted."
	else:
		tarfile.open(ZEROMQ+'.tar.gz').extractall(source_path)
		os.rename(source_path+ZEROMQ, source_path+'zeromq')
		print "   [ZeroMQ] extracted."

	# extract ZeroMQ - C++ binding
	if os.path.exists(source_path+'cppzmq/'):
		print "   [cppzmq] already extracted."
	else:
		tarfile.open(CPPZMQ+'.tar.gz').extractall(source_path)
		print "   [cppzmq] extracted."

def build_sources_iOS(build_type):
	print "\n"
	print "#########################################"
	print "## Trying to build extracted sources ..."
	print "#########################################"
	
	build_path = WORKING_PATH+'built/'
	bin_path = WORKING_PATH+'built/bin/'
	library_path = WORKING_PATH+'built/lib/iOS/'
	include_path = WORKING_PATH+'built/include_ios/SDL2/'

	sdl2_path = WORKING_PATH+'sources/SDL2/Xcode-iOS/SDL/'
	sdl2_include_path = WORKING_PATH+'sources/SDL2/include/'
	sdl2_image_path = WORKING_PATH+'sources/SDL2_image/Xcode-iOS/'
	sdl2_ttf_path = WORKING_PATH+'sources/SDL2_ttf/Xcode-iOS/'
	sdl2_gfx_path = WORKING_PATH+'sources/SDL2_gfx/'
	g3log_path = WORKING_PATH+'sources/g3log/'
	jsoncpp_path = WORKING_PATH+'sources/jsoncpp/'
	pugixml_path = WORKING_PATH+'sources/pugixml/scripts/'
	gtest_path = WORKING_PATH+'sources/gtest/'
	protobuf_path = WORKING_PATH+'sources/protobuf/cmake/'
	zeromq_path = WORKING_PATH+'sources/zeromq/'

	mkdir_p(library_path)

# Copy headers
	if os.path.exists(include_path):
		print "   [SDL2] Header files are exist."
	else:
		print "   [SDL2] Copying header files .."
		copytree(sdl2_include_path, include_path)

		print "   [SDL2_image] Copying header files .."
		copy2(sdl2_image_path+'../SDL_image.h', include_path+'SDL_image.h')

		print "   [SDL2_ttf] Copying header files .."
		copy2(sdl2_ttf_path+'../SDL_ttf.h', include_path+'SDL_ttf.h')

		print "   [SDL2_gfx] Copying header files .."
		copy2(sdl2_gfx_path+'SDL2_framerate.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_gfxPrimitives.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_gfxPrimitives_font.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_imageFilter.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_rotozoom.h', include_path)

	# Overwrite to use iOS header
	copy2(sdl2_include_path+'SDL_config_iphoneos.h', include_path+'SDL_config.h')

# Build SDL2
	if os.path.exists(library_path+'libSDL2.a'):
		print "   [SDL2] already built."
	else:
		os.chdir(sdl2_path)
		os.system(WORKING_PATH+'ios-build.sh SDL2')

# Build SDL2_image
	if os.path.exists(library_path+'libSDL2_image.a'):
		print "   [SDL2_image] already built."
	else:
		os.chdir(sdl2_image_path)
		os.system(WORKING_PATH+'ios-build.sh SDL2_image')

# Build SDL2_ttf
	if os.path.exists(library_path+'libSDL2_ttf.a'):
		print "   [SDL2_ttf] already built."
	else:
		os.chdir(sdl2_ttf_path)
		os.system(WORKING_PATH+'ios-build.sh SDL2_ttf')

# Build SDL2_gfx
	if os.path.exists(library_path+'libSDL2_gfx.a'):
		print "   [SDL2_gfx] already built."
	else:
		os.chdir(sdl2_gfx_path)
		os.system(WORKING_PATH+'ios-build.sh SDL2_gfx armv7')
		os.system(WORKING_PATH+'ios-build.sh SDL2_gfx arm64')

# Build g3log
	if os.path.exists(library_path+'libg3logger.a'):
		print "   [g3log] already built."
	else:
		print "   [g3log] Start building .."
		os.chdir(g3log_path)
		patch_g3log_remove_warnings(g3log_path+'src/')
		# Make fat binary
		os.chdir(g3log_path)
		os.system(WORKING_PATH+'ios-build.sh g3log armv7')
		os.system(WORKING_PATH+'ios-build.sh g3log arm64')

# Generate amalgamated source and header for jsoncpp
	if not os.path.exists(jsoncpp_path+'dist/'):
		print "   [jsoncpp] Copying header files .."
		os.chdir(jsoncpp_path)
		os.system('python amalgamate.py')

# Build pugixml
	if os.path.exists(library_path+'libpugixml.a'):
		print "   [pugixml] already built."
	else:
		print "Start building pugixml .."
		os.chdir(pugixml_path)
		# Make fat binary
		os.system(WORKING_PATH+'ios-build.sh pugixml armv7')
		os.system(WORKING_PATH+'ios-build.sh pugixml arm64')

# Build gtest
	if os.path.exists(library_path+'libgtest.a'):
		print "   [gtest] already built."
	else:
		print "   [gtest] Start building .."
		os.chdir(gtest_path)
		# Make fat binary
		os.system(WORKING_PATH+'ios-build.sh gtest armv7')
		os.system(WORKING_PATH+'ios-build.sh gtest arm64')

# Build protobuf
	if os.path.exists(library_path+'libprotobuf.a'):
		print "   [protobuf] already built."
	else:
		print "   [protobuf] Start building .."
		os.chdir(protobuf_path)
		# Make fat binary
		os.system(WORKING_PATH+'ios-build.sh protobuf armv7')
		os.system(WORKING_PATH+'ios-build.sh protobuf arm64')

# Build ZeroMQ
	if os.path.exists(library_path+'libzmq-static.a'):
		print "   [ZeroMQ] already built."
	else:
		print "   [ZeroMQ] Start building .."
		os.chdir(zeromq_path)
		patch_libzmq_linux(zeromq_path)
		# Make fat binary
		os.system(WORKING_PATH+'ios-build.sh zeromq armv7')
		os.system(WORKING_PATH+'ios-build.sh zeromq arm64')

	print "\n\n"

def build_sources_MSVC(build_type):
	print "\n"
	print "#########################################"
	print "## Trying to build extracted sources ..."
	print "#########################################"
	
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
	gtest_path = WORKING_PATH+'sources/gtest/build/'
	protobuf_path = WORKING_PATH+'sources/protobuf/cmake/build/'
	zeromq_path = WORKING_PATH+'sources/zeromq/builds/msvc/vs2015/'

# Build SDL2
	if os.path.exists(build_path_rel+'SDL2.lib'):
		print "   [SDL2] already built."
	else:
		os.chdir(sdl2_path)
		if STATIC_LINK == True:
			patch_static_MSVC("SDL/SDL.vcxproj")
			patch_static_MSVC("SDLmain/SDLmain.vcxproj")
		os.system('msbuild SDL.sln /t:SDL2;SDL2main /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL.sln /t:SDL2;SDL2main /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "   [SDL2] Copying header files .."
		copytree(sdl2_include_path, include_path)

# Build SDL2_image
	if os.path.exists(build_path_rel+'SDL2_image.lib'):
		print "   [SDL2_image] already built."
	else:
		os.chdir(sdl2_image_path)
		# !REQUIRED! Patch additional library and include path
		#patch_sdl2_image("SDL_image_VS2012.vcxproj")
		patch_sdl2_image("SDL_image.vcxproj")
		if STATIC_LINK == True:
			patch_static_MSVC("SDL_image.vcxproj")
		os.system('msbuild SDL_image.sln /t:SDL2_image /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL_image.sln /t:SDL2_image /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "   [SDL2_image] Copying header files .."
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
		print "   [SDL2_ttf] already built."
	else:
		os.chdir(sdl2_ttf_path)
		# !REQUIRED! Patch additional library and include path
		patch_sdl2_ttf("SDL_ttf.vcxproj")
		if STATIC_LINK == True:
			patch_static_MSVC("SDL_ttf.vcxproj")
		os.system('msbuild SDL_ttf.sln /t:SDL2_ttf /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL_ttf.sln /t:SDL2_ttf /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "   [SDL2_ttf] Copying header files .."
		copy2(sdl2_ttf_path+'../SDL_ttf.h', include_path+'SDL_ttf.h')
		# Copy external libraries
		# TODO: Fix hardcoded arch
		copy2(sdl2_ttf_path+'Win32/Debug/libfreetype-6.dll', build_path_dbg)
		copy2(sdl2_ttf_path+'Win32/Release/libfreetype-6.dll', build_path_rel)

# Build SDL2_gfx
	if os.path.exists(build_path_rel+'SDL2_gfx.lib'):
		print "   [SDL2_gfx] already built."
	else:
		os.chdir(sdl2_gfx_path)
		# !REQUIRED! Patch additional library and include path
		patch_sdl2_gfx("SDL2_gfx.vcxproj")
		if STATIC_LINK == True:
			patch_static_MSVC("SDL2_gfx.vcxproj")
		os.system('msbuild SDL2_gfx.sln /t:SDL2_gfx /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild SDL2_gfx.sln /t:SDL2_gfx /p:PlatformToolSet='+MSVC_VER+' /p:Configuration=Release /p:OutDir='+build_path_rel)
		# Copy headers
		print "   [SDL2_gfx] Copying header files .."
		copy2(sdl2_gfx_path+'SDL2_framerate.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_gfxPrimitives.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_gfxPrimitives_font.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_imageFilter.h', include_path)
		copy2(sdl2_gfx_path+'SDL2_rotozoom.h', include_path)

# Build g3log
	if os.path.exists(build_path_rel+'g3logger.lib'):
		print "   [g3log] already built."
	else:
		print "   [g3log] Start building .."
		mkdir_p(g3log_path)
		os.chdir(g3log_path)
		patch_g3log_remove_warnings(g3log_path)
		# TODO: Check 'CMAKE_BUILD_TYPE' is required if it builds both build type?
		#os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE='+BUILD_CONF+' -G "Visual Studio 12" ..')
		os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DADD_BUILD_WIN_SHARED=ON ..')
		if STATIC_LINK == True:
			patch_static_MSVC("g3logger.vcxproj")
		os.system('msbuild g3log.sln /t:g3logger;g3logger_shared /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild g3log.sln /t:g3logger;g3logger_shared /p:Configuration=Release /p:OutDir='+build_path_rel)

# Generate amalgamated source and header for jsoncpp
	if not os.path.exists(jsoncpp_path+'dist/'):
		print "   [jsoncpp] Copying header files .."
		os.chdir(jsoncpp_path)
		os.system('python amalgamate.py')

# Build gtest
	if os.path.exists(build_path_rel+'gtest.lib'):
		print "   [gtest] already built."
	else:
		print "   [gtest] Start building .."
		mkdir_p(gtest_path)
		os.chdir(gtest_path)
		os.system('cmake ..')
		if STATIC_LINK == False:
			patch_gtest("gtest.vcxproj")
			patch_gtest("gtest_main.vcxproj")
		os.system('msbuild gtest.sln /t:gtest;gtest_main /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild gtest.sln /t:gtest;gtest_main /p:Configuration=Release /p:OutDir='+build_path_rel)

# Build protobuf
	if os.path.exists(build_path_rel+'libprotobuf.lib'):
		print "   [protobuf] already built."
	else:
		print "   [protobuf] Start building .."
		mkdir_p(protobuf_path)
		os.chdir(protobuf_path)
		os.system('cmake .. -Dprotobuf_BUILD_TESTS=OFF')
		os.system('msbuild protobuf.sln /t:libprotobuf /p:Configuration=Debug /p:OutDir='+build_path_dbg)
		os.system('msbuild protobuf.sln /t:libprotobuf /p:Configuration=Release /p:OutDir='+build_path_rel)
		os.rename(build_path_dbg+'libprotobufd.lib', build_path_dbg+'libprotobuf.lib')

# Build zeromq
	if os.path.exists(build_path_rel+'libzmq.lib'):
		print "   [ZeroMQ] is already built."
	else:
		print "   [ZeroMQ] Start building .."
		mkdir_p(zeromq_path)
		os.chdir(zeromq_path)
		# Disable libsodium
		# TODO: Need to check for secure connection
		patch_libzmq_win(zeromq_path+"libzmq/libzmq.props")
		os.system('msbuild libzmq.sln /t:libzmq /p:Option-sodium=false /p:Configuration=DynDebug /p:PlatformToolSet='+MSVC_VER+' /p:OutDir='+build_path_dbg)
		os.system('msbuild libzmq.sln /t:libzmq /p:Option-sodium=false /p:Configuration=DynRelease /p:PlatformToolSet='+MSVC_VER+' /p:OutDir='+build_path_rel)

	print "\n\n"

def build_sources(build_type):
	print "\n"
	print "#########################################"
	print "## Trying to build extracted sources ..."
	print "#########################################"

	build_path = WORKING_PATH+'built/'
	bin_path = WORKING_PATH+'built/bin/'
	library_path = WORKING_PATH+'built/lib/'
	include_path = WORKING_PATH+'built/include/'

	sdl2_path = WORKING_PATH+'sources/SDL2/build/'
	sdl2_image_path = WORKING_PATH+'sources/SDL2_image/build/'
	sdl2_ttf_path = WORKING_PATH+'sources/SDL2_ttf/build/'
	sdl2_gfx_parent_path = WORKING_PATH+'sources/SDL2_gfx/'
	sdl2_gfx_path = WORKING_PATH+'sources/SDL2_gfx/build/'
	g3log_path = WORKING_PATH+'sources/g3log/build/'
	freetype2_path = WORKING_PATH+'sources/SDL2_ttf/external/freetype-2.4.12/build/'
	jsoncpp_path = WORKING_PATH+'sources/jsoncpp/'
	pugixml_path = WORKING_PATH+'sources/pugixml/scripts/build/'
	gtest_path = WORKING_PATH+'sources/gtest/build/'
	protobuf_path = WORKING_PATH+'sources/protobuf/cmake/build/'
	zeromq_path = WORKING_PATH+'sources/zeromq/build/'

	# TODO: Check its working
	if build_type == 'debug':
		DEBUG_BUILD_FLAG = 'CFLAGS="-ggdb3 -O0" CXXFLAGS="-ggdb3 -O0" LDFLAGS="-ggdb3" '
		BUILD_CONF = 'Debug'
	else:
		DEBUG_BUILD_FLAG = ''
		BUILD_CONF = 'Release'
	mkdir_p(build_path)

# Build SDL2
	if os.path.exists(library_path+'libSDL2.a'):
		print "   [SDL2] already built."
	else:
		mkdir_p(sdl2_path)
		os.chdir(sdl2_path)
		os.system(DEBUG_BUILD_FLAG+'../configure --prefix='+build_path+';make -j'+NJOBS+';make install')

	## TODO: iOS build overwrites 'SDL_config.h'
	## Header recover logic is required
	## Make a backup file and overwrite it on macOS build time !

# Build SDL2_image
	if os.path.exists(library_path+'libSDL2_image.a'):
		print "   [SDL2_image] already built."
	else:
		mkdir_p(sdl2_image_path)
		os.chdir(sdl2_image_path)
		os.system(DEBUG_BUILD_FLAG+'PATH='+bin_path+':$PATH ../configure --prefix='+build_path+';make -j'+NJOBS+';make install')

# Build FreeType2
	if os.path.exists(bin_path+'freetype-config'):
		print "   [FreeType2] already built."
	else:
		mkdir_p(freetype2_path)
		os.chdir(freetype2_path)
		os.system(DEBUG_BUILD_FLAG+'PATH='+bin_path+':$PATH ../configure --prefix='+build_path+';make -j'+NJOBS+';make install')

# Build SDL2_ttf
	if os.path.exists(library_path+'libSDL2_ttf.a'):
		print "   [SDL2_ttf] already built."
	else:
		mkdir_p(sdl2_ttf_path)
		os.chdir(sdl2_ttf_path)
		os.system(DEBUG_BUILD_FLAG+'PATH='+bin_path+':$PATH ../configure --prefix='+build_path+';make -j'+NJOBS+';make install')

# Build SDL2_gfx
	if os.path.exists(library_path+'libSDL2_gfx.a'):
		print "   [SDL2_gfx] already built."
	else:
		os.chdir(sdl2_gfx_parent_path)
		os.system('PATH='+bin_path+':$PATH ./autogen.sh --prefix='+build_path)

		mkdir_p(sdl2_gfx_path)
		os.chdir(sdl2_gfx_path)
		os.system(DEBUG_BUILD_FLAG+'PATH='+bin_path+':$PATH ../configure --prefix='+build_path+';make -j'+NJOBS+';make install')

# Build g3log
	if os.path.exists(library_path+'libg3logger.a'):
		print "   [g3log] already built."
	else:
		print "   [g3log] Start building .."
		mkdir_p(g3log_path)
		os.chdir(g3log_path)
		patch_g3log_remove_warnings(g3log_path)
		os.system('cmake -DCHANGE_G3LOG_DEBUG_TO_DBUG=ON -DCMAKE_BUILD_TYPE='+BUILD_CONF+' ..; make g3logger; make g3logger_shared')
		# There is no install rule, just copy library file into built directory.
		copy2(g3log_path+'libg3logger.a', library_path)
		if os.path.exists(g3log_path+'libg3logger.so'):
			copy2(g3log_path+'libg3logger.so', library_path)
		if os.path.exists(g3log_path+'libg3logger_shared.dylib'):
			copy2(g3log_path+'libg3logger_shared.dylib', library_path)

# Generate amalgamated source and header for jsoncpp
	if not os.path.exists(jsoncpp_path+'dist/'):
		print "   [jsoncpp] Copying header files .."
		os.chdir(jsoncpp_path)
		os.system('python amalgamate.py')

# Build pugixml
	if os.path.exists(library_path+'libpugixml.a'):
		print "   [pugixml] already built."
	else:
		print "Start building pugixml .."
		mkdir_p(pugixml_path)
		os.chdir(pugixml_path)
		os.system('cmake -DCMAKE_INSTALL_LIBDIR='+library_path+' -DCMAKE_INSTALL_INCLUDEDIR='+include_path+' ..; make -j'+NJOBS+'; make install')

# Build gtest
	if os.path.exists(library_path+'libgtest.a'):
		print "   [gtest] already built."
	else:
		print "   [gtest] Start building .."
		mkdir_p(gtest_path)
		os.chdir(gtest_path)
		os.system('cmake -DCMAKE_INSTALL_LIBDIR='+library_path+' -DCMAKE_INSTALL_INCLUDEDIR='+include_path+' ..; make -j'+NJOBS+'; make install')
		copy2(gtest_path+'libgtest_main.a', library_path)
		copy2(gtest_path+'libgtest.a', library_path)

# Build protobuf
	if os.path.exists(library_path+'libprotobuf.a'):
		print "   [protobuf] already built."
	else:
		print "   [protobuf] Start building .."
		mkdir_p(protobuf_path)
		os.chdir(protobuf_path)
		os.system('cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR='+library_path+' -DCMAKE_INSTALL_INCLUDEDIR='+include_path+' ..; make libprotobuf -j'+NJOBS)
		copy2(protobuf_path+'libprotobuf.a', library_path)

# Build ZeroMQ
	if os.path.exists(library_path+'libzmq-static.a'):
		print "   [ZeroMQ] already built."
	else:
		print "   [ZeroMQ] Start building .."
		mkdir_p(zeromq_path)
		os.chdir(zeromq_path)
		patch_libzmq_linux(zeromq_path)
		os.system('cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR='+library_path+' -DCMAKE_INSTALL_INCLUDEDIR='+include_path+' ; make libzmq-static -j'+NJOBS)
		copy2(zeromq_path+'lib/libzmq-static.a', library_path)

	print "\n\n"

if __name__ == "__main__":
	# Handle arguments
	parser = argparse.ArgumentParser(description='Build script for external packages !!!NOT supported yet!!!')
	parser.add_argument('--path', help='Set running path of script')
	parser.add_argument('--type', default='debug', choices=['debug', 'release'], help='Select compile type (default: debug)')
	parser.add_argument('--static', default='TRUE', choices=['TRUE', 'FALSE'], help='Select linking type (default: TRUE)')
	parser.add_argument('--arch', default='x86', choices=['x86', 'x64', 'x86_64'], help='Select archtecture for desktop build (default: x86)')
	parser.add_argument('--msvc', default='v140', help='If you use MSVC, you can select MSVC version to build with. (default: v140)')
	parser.add_argument('--platform', default='Windows', choices=['Windows', 'Linux', 'macOS', 'iOS'], help='Select platform to build. (default: Windows)')
	args = parser.parse_args()

	Platform = enum('Windows', 'Linux', 'macOS', 'iOS', 'NotSupport')
	if platform.system().startswith('Linux'):
		CURRENT_PLATFORM = Platform.Linux
	elif platform.system() == 'Windows':
		CURRENT_PLATFORM = Platform.Windows
	elif platform.system() == 'Darwin':
		CURRENT_PLATFORM = Platform.macOS
	else:
		CURRENT_PLATFORM = Platform.NotSupport

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
		WORKING_PATH = os.getcwd() + '/'
	else:
		WORKING_PATH = args.path + '/'

	print "#########################################"
	print "## Prepare external libraries ..."
	print "##"
	print "## Platform    : [ "+Platform.reverse_mapping[CURRENT_PLATFORM]+" ]"
	if CURRENT_PLATFORM == Platform.Windows:
		print "##        MSVC : [ "+args.msvc+" ]"
	#print "## Build type  : [ "+args.type+" ]"
	print "## Static link : [ "+args.static+" ]"
	print "## Working path: [ "+WORKING_PATH+" ]"
	print "## CPU Count   : [ "+NJOBS+" ]"
	print "#########################################"

	# Extract sources from archive files
	extract_sources()

	# Build sources
	if CURRENT_PLATFORM == Platform.Linux:
		build_sources(args.type)
	elif CURRENT_PLATFORM == Platform.Windows:
		WORKING_PATH = WORKING_PATH.replace('\\', '/')
		MSVC_VER = args.msvc
		#if args.static == 'FALSE':
		#	STATIC_LINK = False
		build_sources_MSVC(args.type)
	elif CURRENT_PLATFORM == Platform.macOS:
		build_sources(args.type)
	elif CURRENT_PLATFORM == Platform.iOS:
		print "Build external libs for iOS !"
		build_sources_iOS(args.type)
