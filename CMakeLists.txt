CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(story-project C CXX)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")
SET(LIBNAME "lib${PROJECT_NAME}")
#SET(STATIC_LINKING "TRUE")
SET(STATIC_LINKING "FALSE")

# Detect Platform
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	# Mac OS X
	SET(PLATFORM "macOS")
	SET(PLATFORM_DEF "-DPLATFORM_MACOS")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# Mac OS X
	SET(PLATFORM "Linux")
	SET(PLATFORM_DEF "-DPLATFORM_LINUX")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
IF(WIN32)
	SET(PLATFORM "Windows")
	SET(PLATFORM_DEF "-DPLATFORM_WINDOWS")
ENDIF(WIN32)

# Features
IF(${FEATURE_DISABLE_G3LOG})
ELSE(${FEATURE_DISABLE_G3LOG})
	SET(USE_G3LOG "TRUE")
ENDIF(${FEATURE_DISABLE_G3LOG})

# External cmake files for SDL2
# https://github.com/rodrigo21/sdl2-cmake-scripts
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

# Build external packages
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
	EXECUTE_PROCESS(COMMAND python
		 "${CMAKE_SOURCE_DIR}/external/ext_lib_build.py"
		 --path "${CMAKE_SOURCE_DIR}/external"
		 --platform "${PLATFORM}"
	)
	SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}/external/built/")
ELSEIF(${PLATFORM} STREQUAL "Windows")
	IF(${CMAKE_VERSION} VERSION_GREATER "2.8.10")
		IF(MSVC AND MSVC14)
			SET(VC_VER "--msvc=v140")
			MESSAGE("Visual Studio 14.0 Found.")
			MESSAGE("If build failed, please set environments with below executable.")
			# "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"
			MESSAGE("  C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\VsDevCmd.bat")
			EXECUTE_PROCESS(COMMAND "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\VsDevCmd.bat")
		ELSEIF(MSVC AND MSVC12)
			SET(VC_VER "--msvc=v120")
			MESSAGE("Visual Studio 12.0 Found.")
			MESSAGE("If build failed, please set environments with below executable.")
			MESSAGE("  C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\Tools\\VsDevCmd.bat")
			EXECUTE_PROCESS(COMMAND "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\Tools\\VsDevCmd.bat")
		ELSE(MSVC AND MSVC14)
			MESSAGE("Valid version of Visual Studio was not found.")
			SET(VC_VER " ")
		ENDIF(MSVC AND MSVC14)
	ELSE(${CMAKE_VERSION} VERSION_GREATER "2.8.10")
		SET(VC_VER " ")
	ENDIF(${CMAKE_VERSION} VERSION_GREATER "2.8.10")

	EXECUTE_PROCESS(COMMAND python
		 "${CMAKE_SOURCE_DIR}/external/ext_lib_build.py"
		 --path "${CMAKE_SOURCE_DIR}/external"
		 --static "${STATIC_LINKING}"
		 --platform "${PLATFORM}"
		 ${VC_VER}
	)
	SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}\\external\\built\\")
	SET(SDL2_PATH
		"${CMAKE_SOURCE_DIR}\\external\\built\\Release\\"
		"${CMAKE_SOURCE_DIR}\\external\\built\\Debug\\")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# Search SDL2 libraries
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)
FIND_PACKAGE(SDL2_gfx REQUIRED)

# Search built Python 3
SET(PYTHON_LIBRARY ${CMAKE_SOURCE_DIR}/external/built/lib)
SET(PYTHON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/built/include/python3.6m)
#SET(_Python_PREFIX "${CMAKE_SOURCE_DIR}/external/built/lib")
FIND_PACKAGE(PythonLibs 3.6 REQUIRED)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)
FOREACH(flag ${pkgs_CXXFLAGS})
	SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} ${flag}")
ENDFOREACH(flag)
FOREACH(flag ${pkgs_CXXFLAGS})
	SET(EXTRA_CPPFLAGS "${EXTRA_CPPFLAGS} ${flag}")
ENDFOREACH(flag)

# Strip file name from python lib path
get_filename_component(PYTHON_LIB_PATH ${PYTHON_LIBRARIES} PATH)

# Include and library directories
INCLUDE_DIRECTORIES(
	${SDL2_INCLUDE_DIRS}
	${PYTHON_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/include/
	${CMAKE_SOURCE_DIR}/external/built/include/
	${CMAKE_SOURCE_DIR}/external/built/include/SDL2/
	${CMAKE_SOURCE_DIR}/external/sources/jsoncpp/dist/
	${CMAKE_SOURCE_DIR}/external/sources/pugixml/src/
	${CMAKE_SOURCE_DIR}/external/sources/protobuf/src/
	${CMAKE_SOURCE_DIR}/external/sources/zeromq/include/
	${CMAKE_SOURCE_DIR}/external/sources/cppzmq/
)
IF(USE_G3LOG)
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/external/sources/g3log/src/
)
ELSE(USE_G3LOG)
ADD_DEFINITIONS("-DUSE_SDL_LOG")
ENDIF(USE_G3LOG)
#ADD_DEFINITIONS("-DZMQ_STATIC")
ADD_DEFINITIONS("${PLATFORM_DEF}")

IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
	LINK_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/external/built/lib/
		${CMAKE_BINARY_DIR}
	)
ELSEIF(${PLATFORM} STREQUAL "Windows")
	LINK_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/external/built/lib/$(Configuration)/
		${CMAKE_SOURCE_DIR}/external/built/lib/
		${CMAKE_BINARY_DIR}
		${PYTHON_LIB_PATH}
	)
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# TODO: Below additional flags are not working on MSVC. need check.
IF(${PLATFORM} STREQUAL "Linux")
	SET(ADDITIONAL_CPPFLAGS "-fPIC -Wno-unused-but-set-parameter -Wno-unused-but-set-variable")
	SET(ADDITIONAL_CXXFLAGS "-std=c++11 ${PLATFORM_DEF}")
	# Debug
	SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -g -O0")

	# Python /external/built/lib/python3.6/config-3.6m-x86_64-linux-gnu
	SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -DNDEBUG -fwrapv -Wall -Wstrict-prototypes")
	SET(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS_INIT}
		"-L${PYTHON_LIBRARY}/python3.6/config-3.6m-x86_64-linux-gnu -lpthread -ldl -lutil -lm -lpython3.6m -Xlinker -export-dynamic")
ELSEIF(${PLATFORM} STREQUAL "macOS")
	SET(ADDITIONAL_CPPFLAGS "-fPIC")
	# -Wno-deprecated-declarations for protobuf on macos sierra 10.12.1
	SET(ADDITIONAL_CXXFLAGS "-std=c++11 -Wno-deprecated-declarations ${PLATFORM_DEF}")
	# Debug
	SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -g -O0")

	# Python /external/built/lib/python3.6/config-3.6m-darwin
	SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -DNDEBUG -fwrapv -Wall -Wstrict-prototypes")
	SET(pkgs_LDFLAGS ${pkgs_LDFLAGS} "-L${PYTHON_LIBRARY}/python3.6/config-3.6m-darwin -lpthread -ldl -lutil -lm -lpython3.6m -Xlinker")
ENDIF(${PLATFORM} STREQUAL "Linux")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} ${ADDITIONAL_CFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} ${ADDITIONAL_CXXFLAGS} ")
SET(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} ${EXTRA_CPPFLAGS} ${ADDITIONAL_CPPFLAGS} ")

# Dynamically replace MT / MD for MSVC
IF(MSVC AND STATIC_LINKING)
FOREACH(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	IF(${flag_var} MATCHES "/MD")
		STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	ENDIF(${flag_var} MATCHES "/MD")
ENDFOREACH(flag_var)
ENDIF(MSVC AND STATIC_LINKING)

MESSAGE("############################################")
MESSAGE("  CMake build environments")
MESSAGE("")
MESSAGE("Platform: ${PLATFORM}")
MESSAGE("")
MESSAGE("C_FLAGS      : " ${CMAKE_C_FLAGS})
MESSAGE("CXX_FLAGS    : " ${CMAKE_CXX_FLAGS})
MESSAGE("CPP_FLAGS    : " ${CMAKE_CPP_FLAGS})
MESSAGE("pkgs_LDFLAGS : " ${pkgs_LDFLAGS})
MESSAGE("Python Libs  : " ${PYTHON_LIBRARIES})
MESSAGE("Python Lib path  : " ${PYTHON_LIB_PATH})
MESSAGE("Python include   : " ${PYTHON_INCLUDE_DIRS})
MESSAGE("")
MESSAGE("  Features")
IF(USE_G3LOG)
	MESSAGE("[g3log] : Enabled")
ELSE(USE_G3LOG)
	MESSAGE("[g3log] : Disabled")
ENDIF(USE_G3LOG)
MESSAGE("############################################")

# Main project files
FILE(GLOB_RECURSE TMP_FILES
	"src/*.c"
	"src/*.cpp"
	"scripting/*.cc"
	"scripting/*.cxx"
	"scripting/*.h"
	"include/*.h"
	"include/*.hpp"
)

# Remove single file from list
LIST(REMOVE_ITEM TMP_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Append platform dependent codes
#IF(${PLATFORM} STREQUAL "Windows")
#	LIST(REMOVE_ITEM TMP_FILES "${CMAKE_SOURCE_DIR}/src/util/IPCHelper_nix.cpp")
#ELSE(${PLATFORM} STREQUAL "Windows")
#	LIST(REMOVE_ITEM TMP_FILES "${CMAKE_SOURCE_DIR}/src/util/IPCHelper_win.cpp")
#ENDIF(${PLATFORM} STREQUAL "Windows")

# Remove temporary code
LIST(REMOVE_ITEM TMP_FILES "${CMAKE_SOURCE_DIR}/src/util/IPCHelper_win.cpp")
LIST(REMOVE_ITEM TMP_FILES "${CMAKE_SOURCE_DIR}/src/util/IPCHelper_nix.cpp")

MESSAGE("Compiled code files :")
foreach(f ${TMP_FILES})
	MESSAGE("   ${f}")
endforeach()

# Append external libraries
SET(SOURCES ${TMP_FILES}
	"external/sources/jsoncpp/dist/jsoncpp.cpp"
	"external/sources/pugixml/src/pugixml.cpp"
	"scripting/ipc.pb.cc"
)

# Make source group (a.k.a. filter of MSVC, subdirectory)
foreach(f ${TMP_FILES})
	# Get the path of the file relative to ${DIRECTORY},
	# then alter it (not compulsory)
	file(RELATIVE_PATH SRCGR ${CMAKE_SOURCE_DIR} ${f})
	set(SRCGR "Source Files/${SRCGR}")

	# Extract the folder, ie remove the filename part
	string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

	# Source_group expects \\ (double antislash), not / (slash)
	string(REPLACE / \\ SRCGR ${SRCGR})
	source_group("${SRCGR}" FILES ${f})
endforeach()

##################### Library build with fPIC option

IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
	IF(USE_G3LOG)
		SET(LINK_G3LOG "-lg3logger")
	ELSE(USE_G3LOG)
		SET(LINK_G3LOG "")
	ENDIF(USE_G3LOG)
	ADD_LIBRARY(${LIBNAME} SHARED ${SOURCES})
	SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)
	TARGET_LINK_LIBRARIES(${LIBNAME}
		${pkgs_LDFLAGS}
		${SDL2_LIBRARIES}
		${SDL2_IMAGE_LIBRARIES}
		${SDL2_TTF_LIBRARIES}
		${SDL2_GFX_LIBRARIES}
		${LINK_G3LOG}
		${PYTHON_LIBRARIES}
		"-ldl -lpugixml -lprotobuf -lzmq-static -lboost_python3")
ELSEIF(${PLATFORM} STREQUAL "Windows")
	# Windows
	IF(USE_G3LOG)
		SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/g3logger.lib")
	ELSE(USE_G3LOG)
		SET(LINK_G3LOG "")
	ENDIF(USE_G3LOG)
	ADD_LIBRARY(${LIBNAME} STATIC ${SOURCES})
	SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)
	TARGET_LINK_LIBRARIES(${LIBNAME}
		${pkgs_LDFLAGS}
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2main.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_image.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_ttf.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_gfx.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/libzmq.lib
		${LINK_G3LOG}
		${PYTHON_LIBRARIES}
		"dbghelp.lib")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES PREFIX "" OUTPUT_NAME ${LIBNAME})


##################### Binary build
ADD_EXECUTABLE(${PROJECT_NAME} "src/main.cpp")
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${pkgs_LDFLAGS}
		"-ldl -lprotobuf -lzmq-static -lstory-project")
ELSEIF(${PLATFORM} STREQUAL "Windows")
	# Windows
	IF(USE_G3LOG)
		SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/g3logger.lib")
	ELSE(USE_G3LOG)
		SET(LINK_G3LOG "")
	ENDIF(USE_G3LOG)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${pkgs_LDFLAGS}
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2main.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_image.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_ttf.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_gfx.lib
		${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/libzmq.lib
		${CMAKE_SOURCE_DIR}/build/$(Configuration)/libstory-project.lib
		${LINK_G3LOG}
		"dbghelp.lib")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# Post build
# Copy external dynamic libraries into specific directory to execute process.
IF(${PLATFORM} STREQUAL "Windows")
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ARGS "${CMAKE_SOURCE_DIR}/external/built/$(Configuration)" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)"
	COMMENT "Copy required dynamic libraries")

# Build python binding modules for Windows
#ADD_CUSTOM_COMMAND(TARGET ${LIBNAME}
#	POST_BUILD
#	COMMAND "python.exe"
#	"setup_win.py"
#	"build"
#	--debug
#	"install"
#	--home=build
#	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#	COMMENT "Build python binding modules..."
#)
ELSE(${PLATFORM} STREQUAL "Windows")
# Build python binding modules
ADD_CUSTOM_COMMAND(TARGET ${LIBNAME}
	POST_BUILD
	COMMAND ${CMAKE_SOURCE_DIR}/external/built/bin/python3
	"setup.py"
	"build"
	--debug
	"install"
	--home=build
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Build python binding modules..."
)

# Copy python default modules
#ADD_CUSTOM_COMMAND(TARGET ${LIBNAME}
#	POST_BUILD
#	COMMAND /bin/sh -c "zip -r ${CMAKE_SOURCE_DIR}/build/python36.zip ./*"
#	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/lib/python3.6
#	COMMENT "Copy default python modules"
#)
ENDIF(${PLATFORM} STREQUAL "Windows")

IF(NOT IOS)
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/test)
ENDIF(NOT IOS)

# Build dependency
ADD_DEPENDENCIES(${PROJECT_NAME} ${LIBNAME})

# install
#INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/lib/ DESTINATION ${CMAKE_SOURCE_DIR}/bin
#	FILES_MATCHING PATTERN "*.so*")
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/ DESTINATION ${CMAKE_SOURCE_DIR}/bin
#	FILES_MATCHING PATTERN "*.dll")
#INSTALL(FILES ${CMAKE_SOURCE_DIR}/external/built/*.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin)
