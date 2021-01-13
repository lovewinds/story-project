
# Post build
# Copy external dynamic libraries into specific directory to execute process.
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    SET(INSTALL_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/")
ELSEIF(${PLATFORM} STREQUAL "Windows")
    SET(INSTALL_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/")
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

FILE(GLOB DEPENDENT_FILES
    ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/*.dll
    ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/*.so
)
FILE(COPY ${DEPENDENT_FILES}
     DESTINATION ${INSTALL_TARGET_DIR}
)
MESSAGE("Required dynamic libraries are copied")
# ADD_CUSTOM_COMMAND(
#     TARGET ${PROJECT_NAME}
#     POST_BUILD
#     COMMAND ${PYTHON}
#     ARGS
#         "${CMAKE_SOURCE_DIR}/external/copy-dependencies.py"
#         "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/"
#         "${INSTALL_TARGET_DIR}"
#     COMMENT "Copy required dynamic libraries"
# )

# Copy Python modules
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/python37_modules_Lib.zip"
    "${INSTALL_TARGET_DIR}/python37_modules_Lib.zip"
    COPYONLY
)
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/python37_modules_Tools.zip"
    "${INSTALL_TARGET_DIR}/python37_modules_Tools.zip"
    COPYONLY
)

# Install python modules
IF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))
    MESSAGE("Installing python modules ...")
    ADD_CUSTOM_COMMAND(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/external/build/target/${PLATFORM}/${BUILD_TYPE}/lib/python3.7/lib-dynload/
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/python37-dylib/
    )
ENDIF((${PLATFORM} STREQUAL "Linux") OR (${PLATFORM} STREQUAL "macOS"))

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
