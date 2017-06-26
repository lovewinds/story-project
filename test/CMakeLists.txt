CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(story-test C CXX)

# TODO: Required?
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

FIND_PACKAGE(Threads REQUIRED)

# Include and library directories
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/../include/
    ${CMAKE_SOURCE_DIR}/external/sources/gtest/include/
)
IF(UNIX AND NOT WIN32)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/built/lib/ ${CMAKE_BINARY_DIR})
ELSE(UNIX AND NOT WIN32)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/built/lib/$(Configuration)/ ${CMAKE_BINARY_DIR})
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
    # -lpthread should be last position
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        "-ldl -lgtest -lgtest_main -lstory-project -lpthread")
ELSE(UNIX AND NOT WIN32)
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
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/gtest.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/gtest_main.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/libzmq.lib
        ${CMAKE_SOURCE_DIR}/build/$(Configuration)/libstory-project.lib
        ${LINK_G3LOG}
        "dbghelp.lib")
ENDIF(UNIX AND NOT WIN32)

# Post build
IF(${PLATFORM} STREQUAL "Windows")
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ARGS "${CMAKE_SOURCE_DIR}/external/built/$(Configuration)" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)"
	COMMENT "Copy required dynamic libraries")
ENDIF(${PLATFORM} STREQUAL "Windows")

ADD_DEPENDENCIES(${PROJECT_NAME} ${LIBNAME})
