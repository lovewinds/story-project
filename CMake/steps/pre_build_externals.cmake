## Build external packages
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    EXECUTE_PROCESS(COMMAND ${PYTHON}
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

    EXECUTE_PROCESS(COMMAND ${PYTHON}
         "${CMAKE_SOURCE_DIR}/external/external_build.py"
         --path "${CMAKE_SOURCE_DIR}/external"
         --static "${STATIC_LINKING}"
         --platform "${PLATFORM}"
         --type "${BUILD_TYPE}"
         --msvc "v${MSVC_TOOLSET_VERSION}"
         ${LOG_VERBOSE_FLAG}
         ${VC_VER}
    )
    SET(ENV{SDL2DIR} "${CMAKE_SOURCE_DIR}\\external\\build/target\\${PLATFORM}\\${BUILD_TYPE}\\")
    SET(SDL2_PATH
        "${CMAKE_SOURCE_DIR}\\external\\build/target\\${PLATFORM}\\release\\lib\\"
        "${CMAKE_SOURCE_DIR}\\external\\build/target\\${PLATFORM}\\debug\\lib\\")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

## Prepare library linking
## Search SDL2 libraries
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(SDL2_ttf REQUIRED)
# FIND_PACKAGE(SDL2_gfx REQUIRED)

## Search built Python 3
IF(${PLATFORM} STREQUAL "Windows")
    SET(PYTHON_LIBRARY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/python37.lib)
    SET(PYTHON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/include/python)
ELSEIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    SET(PYTHON_LIBRARY ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/libpython3.7m.a)
    SET(PYTHON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/include/python3.7m)
ENDIF(${PLATFORM} STREQUAL "Windows")
#SET(_Python_PREFIX "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib")
FIND_PACKAGE(PythonLibs 3.7 REQUIRED)

# Strip directory name from PYTHON_LIBRARIES to PYTHON_LIB_PATH
LIST(LENGTH PYTHON_LIBRARIES PYTHON_LIBRARIES_COUNT)
MESSAGE("PYTHON_LIBRARIES       : " ${PYTHON_LIBRARIES})
MESSAGE("PYTHON_LIBRARIES_COUNT : " ${PYTHON_LIBRARIES_COUNT})

IF(PYTHON_LIBRARIES_COUNT LESS_EQUAL "1")
    get_filename_component(PYTHON_LIB_PATH ${PYTHON_LIBRARIES} DIRECTORY)
ELSE(PYTHON_LIBRARIES_COUNT LESS_EQUAL "1")
    # Select 1st item only
    LIST(GET PYTHON_LIBRARIES "1" PYTHON_LIB_ITEM)
    SET(PYTHON_LIBRARIES "${PYTHON_LIB_ITEM}")
    MESSAGE("PYTHON_LIB_ITEM        : " ${PYTHON_LIB_ITEM})
    get_filename_component(PYTHON_LIB_PATH ${PYTHON_LIB_ITEM} DIRECTORY)
ENDIF(PYTHON_LIBRARIES_COUNT LESS_EQUAL "1")
MESSAGE("PYTHON_LIB_PATH        : " ${PYTHON_LIB_PATH})

# Python static build
ADD_DEFINITIONS("-DPy_NO_ENABLE_SHARED")

# Configuration for logging framework
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

ADD_DEFINITIONS("${PLATFORM_DEF}")
