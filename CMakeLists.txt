CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(story-project C CXX)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")
SET(LIBNAME "lib${PROJECT_NAME}")

# Detect Platform
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	# Mac OS X
	SET(PLATFORM "MacOSX")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# Mac OS X
	SET(PLATFORM "Linux")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
IF(WIN32)
	SET(PLATFORM "Windows")
ENDIF(WIN32)

# Features
IF(${FEATURE_DISABLE_G3LOG})
	MESSAGE("Disable g3log")
ELSE(${FEATURE_DISABLE_G3LOG})
	MESSAGE("Enable g3log")
	SET(USE_G3LOG "TRUE")
ENDIF(${FEATURE_DISABLE_G3LOG})

# External cmake files for SDL2
# https://github.com/rodrigo21/sdl2-cmake-scripts
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

MESSAGE("Platform: ${PLATFORM} !")
# Build external packages
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))
	EXECUTE_PROCESS(COMMAND python
		 "${CMAKE_SOURCE_DIR}/external/build_packages.py"
		 --path "${CMAKE_SOURCE_DIR}/external"
		 --platform "${PLATFORM}"
	)
	SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}/external/built/")
ELSEIF(${PLATFORM} STREQUAL "Windows")
	IF(${CMAKE_VERSION} VERSION_GREATER "2.8.10")
		IF(MSVC AND MSVC14)
			SET(VC_VER "--msvc=v140")
			MESSAGE("Visual Studio 14.0 Found.")
		ELSEIF(MSVC AND MSVC12)
			SET(VC_VER "--msvc=v120")
			MESSAGE("Visual Studio 12.0 Found.")
		ELSE(MSVC AND MSVC14)
			MESSAGE("Valid version of Visual Studio was not found.")
			SET(VC_VER " ")
		ENDIF(MSVC AND MSVC14)
	ELSE(${CMAKE_VERSION} VERSION_GREATER "2.8.10")
		SET(VC_VER " ")
	ENDIF(${CMAKE_VERSION} VERSION_GREATER "2.8.10")

	EXECUTE_PROCESS(COMMAND python
		 "${CMAKE_SOURCE_DIR}/external/build_packages.py"
		 --path "${CMAKE_SOURCE_DIR}/external"
		 --platform "${PLATFORM}"
		 ${VC_VER}
	)
	SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}\\external\\built\\")
	SET(SDL2_PATH
		"${CMAKE_SOURCE_DIR}\\external\\built\\Release\\"
		"${CMAKE_SOURCE_DIR}\\external\\built\\Debug\\")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))

# Search SDL2 libraries
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)
FIND_PACKAGE(SDL2_gfx REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)
FOREACH(flag ${pkgs_CXXFLAGS})
	SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} ${flag}")
ENDFOREACH(flag)
FOREACH(flag ${pkgs_CXXFLAGS})
	SET(EXTRA_CPPFLAGS "${EXTRA_CPPFLAGS} ${flag}")
ENDFOREACH(flag)

# Include and library directories
INCLUDE_DIRECTORIES(
	${SDL2_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include/
	${CMAKE_SOURCE_DIR}/external/built/include/
	${CMAKE_SOURCE_DIR}/external/built/include/SDL2/
	${CMAKE_SOURCE_DIR}/external/sources/jsoncpp/dist/
	${CMAKE_SOURCE_DIR}/external/sources/pugixml/src/
)
IF(USE_G3LOG)
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/external/sources/g3log/src/
)
ELSE(USE_G3LOG)
ADD_DEFINITIONS("-DUSE_SDL_LOG")
ENDIF(USE_G3LOG)


IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))
	LINK_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/external/built/lib/
		${CMAKE_BINARY_DIR}
	)
ELSEIF(${PLATFORM} STREQUAL "Windows")
	LINK_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/external/built/lib/$(Configuration)/
		${CMAKE_BINARY_DIR}
	)
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))

# TODO: Below additional flags are not working on MSVC. need check.
IF(${PLATFORM} STREQUAL "Linux")
	SET(ADDITIONAL_CPPFLAGS "-fPIC -Wno-unused-but-set-parameter -Wno-unused-but-set-variable")
	SET(ADDITIONAL_CXXFLAGS "-std=c++11")
ELSEIF(${PLATFORM} STREQUAL "MacOSX")
	SET(ADDITIONAL_CPPFLAGS "-fPIC")
	SET(ADDITIONAL_CXXFLAGS "-std=c++11")
ENDIF(${PLATFORM} STREQUAL "Linux")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} ${ADDITIONAL_CFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} ${ADDITIONAL_CXXFLAGS} ")
SET(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} ${EXTRA_CPPFLAGS} ${ADDITIONAL_CPPFLAGS} ")

MESSAGE("C_FLAGS      : " ${CMAKE_C_FLAGS})
MESSAGE("CXX_FLAGS    : " ${CMAKE_CXX_FLAGS})
MESSAGE("CPP_FLAGS    : " ${CMAKE_CPP_FLAGS})
MESSAGE("pkgs_LDFLAGS : " ${pkgs_LDFLAGS})

# Main project files
FILE(GLOB_RECURSE TMP_FILES
	"src/*.c"
	"src/*.cpp"
	"include/*.h"
	"include/*.hpp"
)

# Append external libraries
SET(SOURCES ${TMP_FILES}
	"external/sources/jsoncpp/dist/jsoncpp.cpp"
	"external/sources/pugixml/src/pugixml.cpp")

# Remove single file from list
LIST(REMOVE_ITEM TMP_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")

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

IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))
	IF(USE_G3LOG)
		SET(LINK_G3LOG "-lg3logger_shared")
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
		"-ldl -lpugixml")
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
		${LINK_G3LOG}
		"dbghelp.lib")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))
SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES PREFIX "" OUTPUT_NAME ${LIBNAME})


##################### Binary build
ADD_EXECUTABLE(${PROJECT_NAME} "src/main.cpp")
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${pkgs_LDFLAGS}
		"-ldl -lstory-project")
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
		${CMAKE_SOURCE_DIR}/build/$(Configuration)/libstory-project.lib
		${LINK_G3LOG}
		"dbghelp.lib")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "MacOSX"))

# Post build
IF(${PLATFORM} STREQUAL "Windows")
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ARGS "${CMAKE_SOURCE_DIR}/external/built/$(Configuration)" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)"
	COMMENT "Copy required dynamic libraries")
ENDIF(${PLATFORM} STREQUAL "Windows")

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/test)

# install
#INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/lib/ DESTINATION ${CMAKE_SOURCE_DIR}/bin
#	FILES_MATCHING PATTERN "*.so*")
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/ DESTINATION ${CMAKE_SOURCE_DIR}/bin
#	FILES_MATCHING PATTERN "*.dll")
#INSTALL(FILES ${CMAKE_SOURCE_DIR}/external/built/*.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin)
