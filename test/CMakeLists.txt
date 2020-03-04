CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(story-test C CXX)

# TODO: Required?
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

FIND_PACKAGE(Threads REQUIRED)

# Detect Platform
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Mac OS X
    SET(PLATFORM "macOS")
    SET(PYTHON "python3")
    SET(PLATFORM_DEF "-DPLATFORM_MACOS")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux
    SET(PLATFORM "Linux")
    SET(PYTHON "python3")
    SET(PLATFORM_DEF "-DPLATFORM_LINUX")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
IF(WIN32)
    SET(PLATFORM "Windows")
    SET(PYTHON "python")
    SET(PLATFORM_DEF "-DPLATFORM_WINDOWS")
ENDIF(WIN32)

# Include and library directories
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/../include/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/gtest/googletest/include/
)
IF(UNIX AND NOT WIN32)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/lib/ ${CMAKE_BINARY_DIR})
ELSE(UNIX AND NOT WIN32)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/ ${CMAKE_BINARY_DIR})
ENDIF(UNIX AND NOT WIN32)

# TODO: Below additional flags are not working on MSVC. need check.
IF(UNIX AND NOT WIN32)
    SET(ADDITIONAL_CXXFLAGS "-Wno-unused-but-set-parameter -Wno-unused-but-set-variable -std=c++11")
ENDIF(UNIX AND NOT WIN32)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CXXFLAGS} ")

MESSAGE(${CMAKE_CXX_FLAGS})
#MESSAGE(${pkgs_LDFLAGS})

# Test project files
FILE(GLOB_RECURSE TEST_FILES
    "${CMAKE_SOURCE_DIR}/test/*.c"
    "${CMAKE_SOURCE_DIR}/test/*.cpp"
    "${CMAKE_SOURCE_DIR}/test/*.h"
    "${CMAKE_SOURCE_DIR}/test/*.hpp"
)

# binary build
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_FILES})
IF(UNIX AND NOT WIN32)
    IF(USE_G3LOG)
        SET(LINK_G3LOG "-lg3logger")
    ELSE(USE_G3LOG)
        SET(LINK_G3LOG "")
    ENDIF(USE_G3LOG)
    # -lpthread should be last position
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        ${LINK_G3LOG}
        "-ldl -lpthread"
        "-lm -lz -lbz2"
        "-lgtest -lgtest_main"
        "-lstory-project"
        # Reference again to resolve linking error
        # https://stackoverflow.com/questions/19901934/libpthread-so-0-error-adding-symbols-dso-missing-from-command-line
        "-lpthread"
    )
ELSE(UNIX AND NOT WIN32)
    # Windows
    IF(USE_G3LOG)
        SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/g3logger.lib")
    ELSE(USE_G3LOG)
        SET(LINK_G3LOG "")
    ENDIF(USE_G3LOG)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2.lib
        # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/SDL2_ttf.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/libfreetype-6.lib
        # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/SDL2_gfx.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/gtest.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/$(Configuration)/lib/gtest_main.lib
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
ENDIF(UNIX AND NOT WIN32)

# Post build
ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${PYTHON}
    ARGS
        "${CMAKE_SOURCE_DIR}/external/copy-dependencies.py"
        "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/"
        "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/"
    COMMENT "Copy required dynamic libraries"
)
# Copy Python modules
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/python37_modules_Lib.zip"
    "${CMAKE_CURRENT_BINARY_DIR}/python37_modules_Lib.zip"
    COPYONLY
)
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/python37_modules_Tools.zip"
    "${CMAKE_CURRENT_BINARY_DIR}/python37_modules_Tools.zip"
    COPYONLY
)

ADD_DEPENDENCIES(${PROJECT_NAME} ${LIBNAME})
