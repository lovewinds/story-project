CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(story-project C CXX)

# TODO: Required?
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

# Check external libraries

# Search SDL2 libraries
# https://github.com/rodrigo21/sdl2-cmake-scripts
# TODO: Does it working on Windows?
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
IF(UNIX AND NOT WIN32)
    MESSAGE("Unix or Linux !")
    EXECUTE_PROCESS(COMMAND python "${CMAKE_SOURCE_DIR}/external/build_packages.py" --path "${CMAKE_SOURCE_DIR}/external/" OUTPUT_VARIABLE EXT_BUILD_RESULT)
    MESSAGE("External script result: ${EXT_BUILD_RESULT}")
    SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}/external/built/")
ELSE(UNIX AND NOT WIN32)
    # Windows
    MESSAGE("Windows !")
    EXECUTE_PROCESS(COMMAND python "${CMAKE_SOURCE_DIR}/external/build_packages.py" --path "${CMAKE_SOURCE_DIR}/external/" OUTPUT_VARIABLE EXT_BUILD_RESULT)
    MESSAGE("External script result: ${EXT_BUILD_RESULT}")
    SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}\\external\\built\\")
    SET(SDL2_PATH "${CMAKE_SOURCE_DIR}\\external\\built\\Release\\"
        "${CMAKE_SOURCE_DIR}\\external\\built\\Debug\\")
ENDIF(UNIX AND NOT WIN32)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)
FIND_PACKAGE(SDL2_gfx REQUIRED)

FOREACH(flag ${pkgs_CXXFLAGS})
    SET(EXTRA_CXXFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

# Include and library directories
INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/external/sources/g3log/src/
    ${CMAKE_SOURCE_DIR}/external/sources/jsoncpp/dist/
    ${CMAKE_SOURCE_DIR}/external/sources/pugixml/src/
)
IF(UNIX AND NOT WIN32)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/built/lib/)
ELSE(UNIX AND NOT WIN32)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/built/lib/$(Configuration)/)
ENDIF(UNIX AND NOT WIN32)

# TODO: Below additional flags are not working on MSVC. need check.
IF(UNIX AND NOT WIN32)
	SET(ADDITIONAL_CXXFLAGS "-Wno-unused-but-set-parameter -Wno-unused-but-set-variable -std=c++11")
ENDIF(UNIX AND NOT WIN32)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CXXFLAGS} ")

MESSAGE(${CMAKE_CXX_FLAGS})
#MESSAGE(${pkgs_LDFLAGS})

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

MESSAGE(${SRCGR})

# binary build
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})
IF(UNIX AND NOT WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_GFX_LIBRARIES}
        "-ldl -lg3logger")
ELSE(UNIX AND NOT WIN32)
    # Windows
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_ttf.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/SDL2_gfx.lib
        ${CMAKE_SOURCE_DIR}/external/built/$(Configuration)/g3logger.lib
        "dbghelp.lib")
ENDIF(UNIX AND NOT WIN32)

# Post build
IF(WIN32)
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ARGS "${CMAKE_SOURCE_DIR}/external/built/$(Configuration)" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)"
    COMMENT "Copy required dynamic libraries")
ENDIF(WIN32)
# install
#INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/lib/ DESTINATION ${CMAKE_SOURCE_DIR}/bin
#	FILES_MATCHING PATTERN "*.so*")
#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/external/built/ DESTINATION ${CMAKE_SOURCE_DIR}/bin
#	FILES_MATCHING PATTERN "*.dll")
#INSTALL(FILES ${CMAKE_SOURCE_DIR}/external/built/*.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin)
