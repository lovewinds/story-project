#!/bin/bash
realpath() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}
SCRIPT_PATH=$(dirname $(realpath $0))

# export PREFIX="story-project/external/build/target/iOS/{release,debug}"
export BINPATH="${CMD_PREFIX}/bin"
export LIBPATH="${CMD_PREFIX}/lib"
export INCLUDEPATH="${CMD_PREFIX}/include"
export IOS32_PREFIX="${CMD_PREFIX}/tmp/ios32"
export IOS64_PREFIX="${CMD_PREFIX}/tmp/ios64"
export SIMULATOR32_PREFIX="${CMD_PREFIX}/tmp/simulator32"
export SIMULATOR64_PREFIX="${CMD_PREFIX}/tmp/simulator64"
export IOS_SIMULATOR_VERSION_MIN=${IOS_SIMULATOR_VERSION_MIN-"8.0"}
export IOS_VERSION_MIN=${IOS_VERSION_MIN-"8.0"}
export XCODEDIR=$(xcode-select -p)

# Build
if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
	export BASEDIR="${XCODEDIR}/Platforms/iPhoneOS.platform/Developer"
	export TARGET="iphoneos"
	export SDK="${BASEDIR}/SDKs/iPhoneOS.sdk"
	export ARCH="arm64"
else
	# export BASEDIR="${XCODEDIR}/Platforms/iPhoneSimulator.platform/Developer"
	# export TARGET="iphonesimulator"
	# export SDK="${BASEDIR}/SDKs/iPhoneSimulator.sdk"
	# export ARCH="x86_64"
	export BASEDIR="${XCODEDIR}/Platforms/iPhoneOS.platform/Developer"
	export TARGET="iphoneos"
	export SDK="${BASEDIR}/SDKs/iPhoneOS.sdk"
	export ARCH="armv7"
fi
export PATH="${BINPATH}:${BASEDIR}/usr/bin:${BASEDIR}/usr/sbin:${PATH}"

## 64-bit iOS
#-fobjc-abi-version=2
export PATH_FRAMEWORK_DEFAULT="${SDK}/System/Library/Frameworks/"
export PATH_FRAMEWORK_USER="${CMD_PREFIX}/frameworks/"
export CC="$(xcrun -sdk ${TARGET} -find clang)"
export CXX="$(xcrun -sdk ${TARGET} -find clang++)"
export AR=$(xcrun -sdk ${TARGET} -find ar)
export RANLIB=$(xcrun -sdk ${TARGET} -find ranlib)
export LIBTOOL=$(xcrun -sdk ${TARGET} -find libtool)

NCPU=`sysctl -n hw.ncpu`
if test x$NJOB = x; then
    NJOB=$NCPU
fi

FRAMEWORK_LIST="-framework Foundation \
-framework UIKit \
-framework OpenGLES \
-framework QuartzCore \
-framework CoreMotion \
-framework GameController \
-framework CoreAudio \
-framework AVFoundation \
-framework CoreGraphics \
-framework AudioToolBox \
-framework ImageIO \
-framework SDL2 \
"

echo "Arch : ${ARCH}"
export CFLAGS="${CFLAGS} -arch ${ARCH} -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
export CXXFLAGS="${CXXFLAGS} -arch ${ARCH} -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
export LDFLAGS="${LDFLAGS} -L${LIBPATH} -isysroot ${SDK} \
	-F${PATH_FRAMEWORK_DEFAULT} -F${PATH_FRAMEWORK_USER} ${FRAMEWORK_LIST}"
#LDFLAGS += -fembed-bitcode

echo "SCRIPT_PATH : ${SCRIPT_PATH}"
echo "PREFIX : ${CMD_PREFIX}"
echo "CFLAGS : ${CFLAGS}"
echo "ARGC : $#"
echo "ARGS : $0"
echo "ARGS : $1"
echo "ARGS : $2"

mkdir -p ${LIBPATH}

if [ 0 -eq $# ]
then
	echo "Usage : $0 [SDL2 | SDL2_image | SDL2_ttf | SDL2_gfx | pugixml | g3log | gtest | protobuf | zeromq] (arm64 | x86_64)"
	exit
fi

### TODO: Copying header files into ${INCLUDEPATH} should be handled first.
### ~/Documents/source/github/story-project/external/built/include
### /Users/ariens/Documents/source/github/story-project/external/built/include
case "$1" in
"SDL2")
	# path : external/build/source/iOS/SDL2/Xcode-iOS/SDL
	xcodebuild -arch ${ARCH} \
		HEADER_SEARCH_PATHS=${INCLUDEPATH} \
		FRAMEWORK_SEARCH_PATHS="${PATH_FRAMEWORK_DEFAULT}"
	# cp build/Release-iphoneos/libSDL2.a ../libSDL2_${ARCH}.a
	cp build/Release-iphoneos/libSDL2.a ${LIBPATH}

	# Make fat binary
	if [ -f "../libSDL2_armv7.a" ] &&  [ -f "../libSDL2_arm64.a" ]; then
		lipo -create \
			../libSDL2_armv7.a \
			../libSDL2_arm64.a \
			-output "${LIBPATH}/libSDL2.a"
	fi
	;;
"SDL2_image")
	# path : external/build/source/iOS/SDL2_image/Xcode-iOS
	xcodebuild -arch ${ARCH} \
		USER_HEADER_SEARCH_PATHS=${INCLUDEPATH} \
		FRAMEWORK_SEARCH_PATHS="${PATH_FRAMEWORK_DEFAULT}"
	# cp build/Release-iphoneos/libSDL2_image.a ../libSDL2_image_${ARCH}.a
	cp build/Release-iphoneos/libSDL2_image.a ${LIBPATH}

	# Make fat binary
	if [ -f "../libSDL2_image_armv7.a" ] &&  [ -f "../libSDL2_image_arm64.a" ]; then
		lipo -create \
			../libSDL2_image_armv7.a \
			../libSDL2_image_arm64.a \
			-output "${LIBPATH}/libSDL2_image.a"
	fi
	;;
"SDL2_ttf")
	# path : external/build/source/iOS/SDL2_ttf/Xcode-iOS
	xcodebuild -arch ${ARCH} \
		HEADER_SEARCH_PATHS_QUOTED_FOR_PROJECT_1=${INCLUDEPATH} \
		FRAMEWORK_SEARCH_PATHS="${PATH_FRAMEWORK_DEFAULT}"
	# cp build/Release-iphoneos/libSDL2_ttf.a ../libSDL2_ttf_${ARCH}.a
	cp build/Release-iphoneos/libSDL2_ttf.a ${LIBPATH}

	# Make fat binary
	if [ -f "../libSDL2_ttf_armv7.a" ] &&  [ -f "../libSDL2_ttf_arm64.a" ]; then
		lipo -create \
			../libSDL2_ttf_armv7.a \
			../libSDL2_ttf_arm64.a \
			-output "${LIBPATH}/libSDL2_ttf.a"
	fi
	;;
"SDL2_gfx")
	./autogen.sh 

	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	export CPPFLAGS="${CPPFLAGS} -I${INCLUDEPATH}/SDL2 -D_THREAD_SAFE"
	export LDFLAGS="${LDFLAGS} -L${LIBPATH} -lSDL2"

	../configure \
		--host=arm-apple-darwin \
		--target=arm-apple-darwin \
		--prefix=${CMD_PREFIX} \
		--disable-mmx \
		--with-sdl-prefix="${INCLUDEPATH}/SDL2" \
		--with-sdl-exec-prefix="${LIBPATH}"
	make -j$NJOB
	cp .libs/libSDL2_gfx.a ../libSDL2_gfx_${ARCH}.a

	# Make fat binary
	if [ -f "../libSDL2_gfx_armv7.a" ] &&  [ -f "../libSDL2_gfx_arm64.a" ]; then
		lipo -create \
			../libSDL2_gfx_armv7.a \
			../libSDL2_gfx_arm64.a \
			-output "${LIBPATH}/libSDL2_gfx.a"
	fi
	;;
"pugixml")
	# Build directory : external/build/source/iOS/pugixml/scripts/build
	cmake .. \
		-DCMAKE_OSX_SYSROOT=${SDK} \
		-DCMAKE_OSX_ARCHITECTURES=${ARCH} \
		-DCMAKE_INSTALL_PREFIX=${CMD_PREFIX}
	make -j$NJOB

	ARCHS=$(lipo -info libpugixml.a | grep 'Architectures')
	if [ "$?" == "0" ]; then
		make install
	fi
	;;

"g3log")
	# Build directory : external/build/source/iOS/g3log/build

	# Build.cmake file of g3log doesn't accept previous CMAKE_CXX_FLAGS.
	# Fix to provide additional flags for cross compile.
	sed -i '' 's/SET(CMAKE_CXX_FLAGS \"-Wall/SET(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -Wall/' ../Build.cmake

	cmake .. \
		-DCMAKE_OSX_SYSROOT=${SDK} \
		-DCMAKE_OSX_ARCHITECTURES=${ARCH} \
		-DCHANGE_G3LOG_DEBUG_TO_DBUG=ON \
		-DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
		-DCMAKE_INSTALL_PREFIX=${CMD_PREFIX}
	make g3logger -j$NJOB

	ARCHS=$(lipo -info libg3logger.a | grep 'Architectures')
	if [ "$?" == "0" ]; then
		make install
	fi
	;;

"gtest")
	# Build directory : external/build/source/iOS/gtest/build
	cmake .. \
		-DCMAKE_OSX_SYSROOT=${SDK} \
		-DCMAKE_OSX_ARCHITECTURES=${ARCH} \
		-DCMAKE_INSTALL_PREFIX=${CMD_PREFIX}
	make -j$NJOB

	ARCHS=$(lipo -info googlemock/gtest/libgtest.a | grep 'Architectures')
	if [ "$?" == "0" ]; then
		make install
	fi
	;;

"protobuf")
	# Build directory : external/build/source/iOS/protobuf/cmake/build
	cmake .. \
		-DCMAKE_OSX_SYSROOT=${SDK} \
		-Wno-dev \
		-DCMAKE_OSX_ARCHITECTURES=${ARCH} \
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
		-Dprotobuf_BUILD_TESTS=OFF \
		-DCMAKE_INSTALL_PREFIX=${CMD_PREFIX}
	make libprotobuf -j$NJOB

	ARCHS=$(lipo -info libprotobuf.a | grep 'Architectures')
	if [ "$?" == "0" ]; then
		make install
	fi
	;;

"zeromq")
	# Build directory : external/build/source/iOS/zeromq/build
	rm -r ./*
	cmake .. \
		-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
		-DCMAKE_OSX_SYSROOT=${SDK} \
		-DZMQ_BUILD_TESTS=OFF \
		-DCMAKE_INSTALL_PREFIX=${CMD_PREFIX}
	make -j$NJOB
	cp lib/libzmq.a ../libzmq_${ARCH}.a

	# Make fat binary
	if [ -f "../libzmq_armv7.a" ] && [ -f "../libzmq_arm64.a" ]; then
		lipo -create \
			../libzmq_armv7.a \
			../libzmq_arm64.a \
			-output "${LIBPATH}/libzmq.a"
	fi
	;;

*)
	echo "Package [$1] not found from list."
	echo "Usage : $0 [SDL2 | SDL2_image | SDL2_ttf | SDL2_gfx | pugixml | g3log | gtest | protobuf | zeromq] (armv7 | arm64)"
esac




#SDL2_gfx
#######./autogen.sh

#######rm -rf build32
#######mkdir build32
#######cd build32

#../configure --host=armv7-apple-darwin --target=arm-apple-darwin --prefix=${CMD_PREFIX}

#pugixml
#cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKEƒ_INSTALL_INCLUDEDIR=${INCLUDEPATH}

#SDL2_gfx
#######../configure --host=armv7-apple-darwin --target=arm-apple-darwin --prefix=${CMD_PREFIX} --disable-mmx


#cp ../include/SDL_config_iphoneos.h ../include/SDL_config.h
#../configure --host=aarch64-apple-darwin11-lipo --target=arm-apple-darwin --prefix=${CMD_PREFIX}

#######make
#make install


#xcodebuild -arch arm64 -arch armv7

# SDL2
#xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS=${INCLUDEPATH}/SDL2/
###xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS=~/Documents/source/github/story-project/external/built/include/SDL2/

# SDL2_image
#xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS=${INCLUDEPATH}/SDL2/:${INCLUDEPATH}

# SDL2_ttf
#xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS_QUOTED_FOR_PROJECT_1=${INCLUDEPATH}/SDL2/