CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(story-project C CXX)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")
SET(LIBNAME "lib${PROJECT_NAME}")
SET(STATIC_LINKING "TRUE")
# SET(STATIC_LINKING "FALSE")
OPTION(VERBOSE "Displays entire build log" OFF)

# Build Type
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE Release ... FORCE )
ENDIF()
SET(BUILD_TYPE "release")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(BUILD_TYPE "debug")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

# Log verbosity
SET(LOG_VERBOSE_FLAG "")
IF(VERBOSE)
    SET(LOG_VERBOSE_FLAG "--verbose")
ENDIF()

# Detect Platform
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Mac OS X
    SET(PLATFORM "macOS")
    SET(PLATFORM_DEF "-DPLATFORM_MACOS")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux
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

SET(Python_USE_STATIC_LIBS "TRUE")

# Build external packages
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    EXECUTE_PROCESS(COMMAND python
        "${CMAKE_SOURCE_DIR}/external/external_build.py"
        --path "${CMAKE_SOURCE_DIR}/external"
        --platform "${PLATFORM}"
        --type "${BUILD_TYPE}"
        ${LOG_VERBOSE_FLAG}
    )
    SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/")
ELSEIF(${PLATFORM} STREQUAL "Windows")
    IF(MSVC AND MSVC141)
        SET(VC_VER "--msvc=v${MSVC_TOOLSET_VERSION}")
        MESSAGE("Visual Studio 14.1 Found.")
    ELSEIF(MSVC AND MSVC14)
        SET(VC_VER "--msvc=v${MSVC_TOOLSET_VERSION}")
        MESSAGE("Visual Studio 14.0 Found.")
        MESSAGE("If build failed, please set environments with below executable.")
        # "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"
        # "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
        MESSAGE("  C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\VsDevCmd.bat")
        EXECUTE_PROCESS(COMMAND "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\VsDevCmd.bat")
    ELSEIF(MSVC AND MSVC12)
        SET(VC_VER "--msvc=v${MSVC_TOOLSET_VERSION}")
        MESSAGE("Visual Studio 12.0 Found.")
        MESSAGE("If build failed, please set environments with below executable.")
        MESSAGE("  C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\Tools\\VsDevCmd.bat")
        EXECUTE_PROCESS(COMMAND "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\Common7\\Tools\\VsDevCmd.bat")
    ELSE(MSVC AND MSVC14)
        MESSAGE("Valid version of Visual Studio was not found.")
        SET(VC_VER " ")
    ENDIF(MSVC AND MSVC141)

    EXECUTE_PROCESS(COMMAND python
         "${CMAKE_SOURCE_DIR}/external/external_build.py"
         --path "${CMAKE_SOURCE_DIR}/external"
         --static "${STATIC_LINKING}"
         --platform "${PLATFORM}"
         --type "${BUILD_TYPE}"
         ${LOG_VERBOSE_FLAG}
         ${VC_VER}
    )
    SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}\\external\\build/target\\${PLATFORM}\\${BUILD_TYPE}\\")
    SET(SDL2_PATH
        "${CMAKE_SOURCE_DIR}\\external\\build/target\\${PLATFORM}\\release\\lib\\"
        "${CMAKE_SOURCE_DIR}\\external\\build/target\\${PLATFORM}\\debug\\lib\\")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# Search SDL2 libraries
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)
# FIND_PACKAGE(SDL2_gfx REQUIRED)

# Search built Python 3
IF(${PLATFORM} STREQUAL "Windows")
    SET(PYTHON_LIBRARY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/python37.lib)
    SET(PYTHON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/include/python)
ELSEIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    SET(PYTHON_LIBRARY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/libpython3.7m.a)
    SET(PYTHON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/include/python3.7m)
ENDIF(${PLATFORM} STREQUAL "Windows")

#SET(_Python_PREFIX "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib")
FIND_PACKAGE(PythonLibs 3.7 REQUIRED)

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
    ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/include/
    # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/include/SDL2/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/jsoncpp/dist/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/pugixml/src/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/protobuf/src/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/zeromq/include/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/cppzmq/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/pybind11/include/
)
IF(USE_G3LOG)
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/g3log/src/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/g3log/build/include
)
ELSE(USE_G3LOG)
ADD_DEFINITIONS("-DUSE_SDL_LOG")
ENDIF(USE_G3LOG)

# Required if use zmq static linking
ADD_DEFINITIONS("-DZMQ_STATIC")

# Python static build
ADD_DEFINITIONS("-DPy_NO_ENABLE_SHARED")

ADD_DEFINITIONS("${PLATFORM_DEF}")

IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/include/
    )
    LINK_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/
        ${CMAKE_BINARY_DIR}
    )
ELSEIF(${PLATFORM} STREQUAL "Windows")
    # NOMINMAX for pybind11 on windows build
    ADD_DEFINITIONS("-DNOMINMAX")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /MP /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /MP /EHsc")
    INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/dirent/include/
        ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/python/include/
        ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/python/Python/
        ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/python/PC/
        ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/python/Modules/
    )
    LINK_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/
        ${CMAKE_BINARY_DIR}
        ${PYTHON_LIB_PATH}
    )
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# TODO: Below additional flags are not working on MSVC. need check.
IF(${PLATFORM} STREQUAL "Linux")
    SET(ADDITIONAL_CPPFLAGS "-Wno-unused-but-set-parameter -Wno-unused-but-set-variable")
    SET(ADDITIONAL_CXXFLAGS "-std=c++11 ${PLATFORM_DEF}")
    # Debug
    SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -g -O0")

    # Python /external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/python3.6/config-3.6m-x86_64-linux-gnu
    # 	"-L${PYTHON_LIBRARY}/python3.6/config-3.6m-x86_64-linux-gnu -lpthread -ldl -lutil -lm -lpython3.6m -Xlinker -export-dynamic")
    SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -DNDEBUG -fwrapv -Wall -fPIC")
    # SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_INIT}
    # 	" -lpthread -ldl -lutil -lm -Xlinker -export-dynamic")
    #  -lpython3.6m
    SET(pkgs_LDFLAGS ${pkgs_LDFLAGS} "-lpthread -ldl -lutil -lm")
ELSEIF(${PLATFORM} STREQUAL "macOS")
    # -Wno-deprecated-declarations for protobuf on macos sierra 10.12.1
    SET(ADDITIONAL_CXXFLAGS "-std=c++11 -Wno-deprecated-declarations ${PLATFORM_DEF}")
    # Debug
    SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -g -O0")

    # Python /external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/python3.6/config-3.6m-darwin
    SET(ADDITIONAL_CXXFLAGS "${ADDITIONAL_CXXFLAGS} -DNDEBUG -fwrapv -Wall -fPIC")
    SET(pkgs_LDFLAGS ${pkgs_LDFLAGS} "-lpthread -ldl -lutil -lm")
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
        MESSAGE(${flag_var})
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
MESSAGE("Python Libs     : " ${PYTHON_LIBRARIES})
MESSAGE("Python Lib path : " ${PYTHON_LIB_PATH})
MESSAGE("Python include  : " ${PYTHON_INCLUDE_DIRS})
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
    "external/build/source/${PLATFORM}/jsoncpp/dist/jsoncpp.cpp"
    "external/build/source/${PLATFORM}/pugixml/src/pugixml.cpp"
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
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        # ${SDL2_GFX_LIBRARIES}
        ${LINK_G3LOG}
        ${PYTHON_LIBRARIES}
        # -lpython3.6m -lutil
        "${pkgs_LDFLAGS}"
        "-ldl -lpugixml -lprotobuf -lzmq")
ELSEIF(${PLATFORM} STREQUAL "Windows")
    # Windows
    IF(USE_G3LOG)
        SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/g3logger.lib")
    ELSE(USE_G3LOG)
        SET(LINK_G3LOG "")
    ENDIF(USE_G3LOG)
    ADD_LIBRARY(${LIBNAME} STATIC ${SOURCES})
    SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)
    TARGET_LINK_LIBRARIES(${LIBNAME}
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_ttf.lib
        # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_gfx.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/libzmq.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/python37.lib
        ${LINK_G3LOG}
        "${pkgs_LDFLAGS}"
        "winmm.lib"
        "version.lib"
        "imm32.lib"
        "setupapi.lib"
        "opengl32.lib"
        "glu32.lib"
        "dbghelp.lib"
        # for ZeroMQ
        "wsock32.lib"
        "ws2_32.lib"
        "Iphlpapi.lib"
        # for Python
        "shlwapi.lib"
    )
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES PREFIX "" OUTPUT_NAME ${LIBNAME})


##################### Binary build
ADD_EXECUTABLE(${PROJECT_NAME} "src/main.cpp")

IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        "-ldl -lprotobuf -lzmq -lstory-project")
ELSEIF(${PLATFORM} STREQUAL "Windows")
    # Windows
    IF(USE_G3LOG)
        SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/g3logger.lib")
    ELSE(USE_G3LOG)
        SET(LINK_G3LOG "")
    ENDIF(USE_G3LOG)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_ttf.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/libfreetype-6.lib
        # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_gfx.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/libzmq.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/python37.lib
        ${CMAKE_SOURCE_DIR}/build/$(Configuration)/libstory-project.lib
        ${LINK_G3LOG}
        "winmm.lib"
        "version.lib"
        "imm32.lib"
        "setupapi.lib"
        "opengl32.lib"
        "glu32.lib"
        "dbghelp.lib"
        # for ZeroMQ
        "wsock32.lib"
        "ws2_32.lib"
        "Iphlpapi.lib"
        # for Python
        "shlwapi.lib"
    )
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# Post build
# Copy external dynamic libraries into specific directory to execute process.
ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND python
    ARGS
        "${CMAKE_SOURCE_DIR}/external/copy-dependencies.py"
        "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/"
        "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/"
    COMMENT "Copy required dynamic libraries"
)
# Copy 
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/*.zip"
    "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/"
    COPYONLY
)

# Build python binding modules
IF(${PLATFORM} STREQUAL "Windows")
    # for Windows
    # ADD_CUSTOM_COMMAND(TARGET ${LIBNAME}
    # 	POST_BUILD
    # 	COMMAND "python.exe"
    # 	"setup_win.py"
    # 	"build"
    # 	--debug
    # 	"install"
    # 	--home=build
    # 	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    # 	COMMENT "Build python binding modules..."
    # )
ELSE(${PLATFORM} STREQUAL "Windows")
    ADD_CUSTOM_COMMAND(TARGET ${LIBNAME}
        POST_BUILD
        COMMAND _PLATFORM=${PLATFORM} ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/bin/python3
        "setup.py"
        "build"
        --debug
        "install"
        --home=build
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Build python binding modules..."
    )
    # # Copy python default modules
    # ADD_CUSTOM_COMMAND(TARGET ${LIBNAME}
    #     POST_BUILD
    #     COMMAND /bin/sh -c "zip -r ${CMAKE_SOURCE_DIR}/build/python37.zip ./*"
    #     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/python3.7
    #     COMMENT "Copy default python modules"
    # )
ENDIF(${PLATFORM} STREQUAL "Windows")

# For test
IF(NOT IOS)
    ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/test)
ENDIF(NOT IOS)

# Build dependency
ADD_DEPENDENCIES(${PROJECT_NAME} ${LIBNAME})

# Install
# INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
# INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/lib/
#     DESTINATION ${CMAKE_SOURCE_DIR}/bin
#    FILES_MATCHING PATTERN "*.so*")
# INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/
#     DESTINATION ${CMAKE_SOURCE_DIR}/bin
#     FILES_MATCHING PATTERN "*.dll")
# INSTALL(FILES ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/*.dll
#     DESTINATION ${CMAKE_SOURCE_DIR}/bin)
