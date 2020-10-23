
# Include and library directories
INCLUDE_DIRECTORIES(
    ${SDL2_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/include/python
    # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/include/SDL2/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/jsoncpp/dist/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/pugixml/src/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/protobuf/src/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/zeromq/include/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/cppzmq/
    ${CMAKE_SOURCE_DIR}/external/build/source/${PLATFORM}/pybind11/include/
)

IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/include/
    )
    LINK_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/
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
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/
        ${CMAKE_BINARY_DIR}
        ${PYTHON_LIB_PATH}
    )
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

# TODO: Below additional flags are not working on MSVC. need check.
IF(${PLATFORM} STREQUAL "Linux")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -std=c++11")
    # Debug
    STRING(APPEND ADDITIONAL_CXXFLAGS " -g -O0")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -DNDEBUG -fwrapv -Wall -fPIC")
    # STRING(APPEND ADDITIONAL_CXXFLAGS " -Werror")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unknown-pragmas")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unused-variable")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unused-but-set-parameter")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unused-but-set-variable")
    STRING(APPEND ADDITIONAL_CXXFLAGS " ${PLATFORM_DEF}")
    SET(pkgs_LDFLAGS ${pkgs_LDFLAGS} "-lpthread -ldl -lutil -lm -lz")
ELSEIF(${PLATFORM} STREQUAL "macOS")
    # -Wno-deprecated-declarations for protobuf on macos sierra 10.12.1
    STRING(APPEND ADDITIONAL_CXXFLAGS " -std=c++11")
    # Debug
    STRING(APPEND ADDITIONAL_CXXFLAGS " -g -O0")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -DNDEBUG -fwrapv -Wall -fPIC")
    # STRING(APPEND ADDITIONAL_CXXFLAGS " -Werror")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unknown-pragmas")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-deprecated-declarations")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unused-variable")
    STRING(APPEND ADDITIONAL_CXXFLAGS " -Wno-unused-parameter")
    STRING(APPEND ADDITIONAL_CXXFLAGS " ${PLATFORM_DEF}")
    SET(pkgs_LDFLAGS ${pkgs_LDFLAGS} "-lpthread -ldl -lutil -lm -lz")
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
        # MESSAGE(${flag_var})
        STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    ENDIF(${flag_var} MATCHES "/MD")
ENDFOREACH(flag_var)

ENDIF(MSVC AND STATIC_LINKING)


MESSAGE("############################################")
MESSAGE("  CMake build environments")
MESSAGE("")
MESSAGE("Platform :: ${PLATFORM}")
MESSAGE("")
MESSAGE("C_FLAGS      : " ${CMAKE_C_FLAGS})
MESSAGE("CXX_FLAGS    : " ${CMAKE_CXX_FLAGS})
MESSAGE("CPP_FLAGS    : " ${CMAKE_CPP_FLAGS})
MESSAGE("pkgs_LDFLAGS : " ${pkgs_LDFLAGS})
MESSAGE("Python Libs     : " ${PYTHON_LIBRARIES})
MESSAGE("Python Lib path : " ${PYTHON_LIB_PATH})
MESSAGE("Python include  : " ${PYTHON_INCLUDE_DIRS})
MESSAGE("")
MESSAGE("Features ::")
IF(USE_G3LOG)
    MESSAGE("  [g3log] : Enabled")
    MESSAGE("            ${G3LOG_INCLUDE_DIR}")
ELSE(USE_G3LOG)
    MESSAGE("  [g3log] : Disabled")
ENDIF(USE_G3LOG)
MESSAGE("############################################")

# Main project files
FILE(GLOB_RECURSE TMP_FILES
    "src/*.c"
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "scripting/*.cc"
    "scripting/*.cxx"
    "scripting/*.h"
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

# MESSAGE("Compiled code files :")
# foreach(f ${TMP_FILES})
#     MESSAGE("   ${f}")
# endforeach()

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
        "-ldl -lpthread"
        "-lbz2 -lz"
        "-lpugixml -lprotobuf -lzmq"
    )
ELSEIF(${PLATFORM} STREQUAL "Windows")
    # Windows
    IF(USE_G3LOG)
        SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/g3logger.lib")
    ELSE(USE_G3LOG)
        SET(LINK_G3LOG "")
    ENDIF(USE_G3LOG)
    ADD_LIBRARY(${LIBNAME} STATIC ${SOURCES})
    SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES LINKER_LANGUAGE CXX)
    TARGET_LINK_LIBRARIES(${LIBNAME}
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2_ttf.lib
        # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2_gfx.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/libzmq.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/python37.lib
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
        SET(LINK_G3LOG "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/g3logger.lib")
    ELSE(USE_G3LOG)
        SET(LINK_G3LOG "")
    ENDIF(USE_G3LOG)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${pkgs_LDFLAGS}
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2_image.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2_ttf.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/libfreetype-6.lib
        # ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/SDL2_gfx.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/libzmq.lib
        ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE_DIR}/lib/python37.lib
        ${CMAKE_SOURCE_DIR}/build/${BUILD_TYPE_DIR}/libstory-project.lib
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
