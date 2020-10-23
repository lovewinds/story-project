CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(story-project C CXX)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/src")
SET(LIBNAME "lib${PROJECT_NAME}")
SET(STATIC_LINKING "TRUE")
# SET(STATIC_LINKING "FALSE")
OPTION(VERBOSE "Displays entire build log" OFF)

# Build Type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release CACHE STRING "Set default build type as Release" FORCE)
ENDIF()
SET(BUILD_TYPE "release")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(BUILD_TYPE "debug")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

# ============================================================================
# Basic build configurations
# ============================================================================
INCLUDE(${CMAKE_SOURCE_DIR}/CMake/steps/configuration.cmake)


# ============================================================================
# Build external packages
# ============================================================================
INCLUDE(${CMAKE_SOURCE_DIR}/CMake/steps/pre_build_externals.cmake)


# ============================================================================
# Main build steps
# ============================================================================
INCLUDE(${CMAKE_SOURCE_DIR}/CMake/steps/build.cmake)


# ============================================================================
# Post build steps
# ============================================================================
INCLUDE(${CMAKE_SOURCE_DIR}/CMake/steps/post_build.cmake)


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
