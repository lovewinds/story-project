CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(story-project CXX)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

# Set required packages
INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED sdl2 SDL2_image SDL2_ttf SDL2_gfx)

FOREACH(flag ${pkgs_CXXFLAGS})
	SET(EXTRA_CXXFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

INCLUDE_DIRECTORIES(
#	/usr/local/include/SDL2/
	/usr/include/SDL2/
	${CMAKE_SOURCE_DIR}/include/
)

SET(ADDITIONAL_CXXFLAGS "-Wno-unused-but-set-parameter -Wno-unused-but-set-variable")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CXXFLAGS}")

MESSAGE(${CMAKE_CXX_FLAGS})
MESSAGE(${pkgs_LDFLAGS})

SET(SRCS
	src/main.cpp
	src/Ecore.cpp
	src/EWindow.cpp
	src/EScreenManager.cpp
	src/drawable/EDrawable.cpp
	src/drawable/EFigure.cpp
	src/drawable/ETexture.cpp
	src/drawable/ETextureHandler.cpp
	src/drawable/EImageTexture.cpp
	src/drawable/ETextTexture.cpp
	src/drawable/ESprite.cpp
)

# binary build
ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS} "-ldl")

# install
#INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
#INSTALL(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION /usr/share/license RENAME telephony-daemon)
