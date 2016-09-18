export SRC="/Users/ariens/Documents/source/github/story-project"
export PREFIX="${SRC}/external/built"
export BINPATH="${SRC}/external/built/bin"
export LIBPATH="${SRC}/external/built/lib"
export INCLUDEPATH="${SRC}/external/built/include"
export IOS32_PREFIX="$PREFIX/tmp/ios32"
export IOS64_PREFIX="$PREFIX/tmp/ios64"
export SIMULATOR32_PREFIX="$PREFIX/tmp/simulator32"
export SIMULATOR64_PREFIX="$PREFIX/tmp/simulator64"
export IOS_SIMULATOR_VERSION_MIN=${IOS_SIMULATOR_VERSION_MIN-"8.0"}
export IOS_VERSION_MIN=${IOS_VERSION_MIN-"8.0"}
export XCODEDIR=$(xcode-select -p)

# Build
export BASEDIR="${XCODEDIR}/Platforms/iPhoneOS.platform/Developer"
export PATH="${BINPATH}:${BASEDIR}/usr/bin:$BASEDIR/usr/sbin:$PATH"

## 64-bit iOS
#-fobjc-abi-version=2
export SDK="${BASEDIR}/SDKs/iPhoneOS.sdk"
export FRAMEWORKS="${SDK}/System/Library/Frameworks/"
export CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
export CPP="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -E"
export AR="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar"
export RANLIB="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib"
export LIBTOOL=$(xcrun -sdk iphoneos -find libtool)
#export AR=$(xcrun -sdk iphoneos -find ar)
#export RANLIB=$(xcrun -sdk iphoneos -find ranlib)

#armv7 arm64
if [ 2 -eq $# ] && [ "$2" = "arm64" ]
then
	echo "Arch : arm64"
	export CFLAGS="${CFLAGS} -arch arm64 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
	export CXXFLAGS="${CXXFLAGS} -arch arm64 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
	export CPPFLAGS="${CPPFLAGS} -arch arm64 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
	export LDFLAGS="${LDFLAGS} -L${LIBPATH} -isysroot ${SDK} -F${FRAMEWORKS} -framework Foundation -framework UIKit -framework OpenGLES -framework QuartzCore -framework CoreMotion -framework GameController -framework CoreAudio -framework AudioToolbox -framework CoreGraphics -framework ImageIO -fembed-bitcode"
else
	echo "Arch : armv7"
	export CFLAGS="${CFLAGS} -arch armv7 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
	export CXXFLAGS="${CXXFLAGS} -arch armv7 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
	export CPPFLAGS="${CPPFLAGS} -arch armv7 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
	export LDFLAGS="${LDFLAGS} -L${LIBPATH} -isysroot ${SDK} -F${FRAMEWORKS} -framework Foundation -framework UIKit -framework OpenGLES -framework QuartzCore -framework CoreMotion -framework GameController -framework CoreAudio -framework AudioToolbox -framework CoreGraphics -framework ImageIO -fembed-bitcode"
fi

echo "CFLAGS : ${CFLAGS}"
echo "ARGC : $#"
echo "ARGS : $0"
echo "ARGS : $1"
echo "ARGS : $2"

if [ 0 -eq $# ]
then
	echo "Usage : $0 [SDL2 | SDL2_image | SDL2_ttf | SDL2_gfx | pugixml | g3log | gtest | protobuf | zeromq] (armv7 | arm64)"
	exit
fi

### TODO: Copying header files into ${INCLUDEPATH} should be handled first.
### ~/Documents/source/github/story-project/external/built/include
### /Users/ariens/Documents/source/github/story-project/external/built/include
case "$1" in
"SDL2")
	# path : external/sources/SDL2/Xcode-iOS/SDL
	xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS=${INCLUDEPATH}/SDL2
	cp build/Release-iphoneos/libSDL2.a ${LIBPATH}
	;;
"SDL2_image")
	# path : external/sources/SDL2_image/Xcode-iOS
	xcodebuild -arch arm64 -arch armv7 USER_HEADER_SEARCH_PATHS=${INCLUDEPATH}/SDL2
	cp build/Release-iphoneos/libSDL2_image.a ${LIBPATH}
	;;
"SDL2_ttf")
	# path : external/sources/SDL2_ttf/Xcode-iOS
	xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS_QUOTED_FOR_PROJECT_1=${INCLUDEPATH}/SDL2/
	cp build/Release-iphoneos/libSDL2_ttf.a ${LIBPATH}
	;;
"SDL2_gfx")
	./autogen.sh

	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
		../configure --host=arm-apple-darwin --target=arm-apple-darwin --prefix=${PREFIX} --disable-mmx
		make
		cp .libs/libSDL2_gfx.a ../libSDL2_gfx_arm64.a
	else
		../configure --host=arm-apple-darwin --target=arm-apple-darwin --prefix=${PREFIX} --disable-mmx
		make
		cp .libs/libSDL2_gfx.a ../libSDL2_gfx_armv7.a
	fi
	#lipo -create ../libSDL2_gfx_armv7.a ../libSDL2_gfx_arm64.a -output=${LIBPATH}/libSDL2_gfx.a
	;;
"pugixml")
	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
		cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make
		cp libpugixml.a ../libpugixml_arm64.a
	else
		cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make
		cp libpugixml.a ../libpugixml_armv7.a
	fi
	#lipo -create ../libSDL2_gfx_armv7.a ../libSDL2_gfx_arm64.a -output=${LIBPATH}/libSDL2_gfx.a
	;;

"g3log")
	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
		cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH} -DCMAKE_CXX_FLAGS="${CXXFLAGS}"
		make
		cp libg3logger.a ../libg3logger_arm64.a
	else
		cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH} -DCMAKE_CXX_FLAGS="${CXXFLAGS}"
		make
		cp libg3logger.a ../libg3logger_armv7.a
	fi
	#lipo -create ../libSDL2_gfx_armv7.a ../libSDL2_gfx_arm64.a -output=${LIBPATH}/libSDL2_gfx.a
	;;

"gtest")
	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
		cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make
		cp libgtest.a ../libgtest_arm64.a
	else
		cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make
		cp libgtest.a ../libgtest_armv7.a
	fi
	#lipo -create ../libSDL2_gfx_armv7.a ../libSDL2_gfx_arm64.a -output=${LIBPATH}/libSDL2_gfx.a
	;;

"protobuf")
	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
		cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make libprotobuf
		cp libprotobuf.a ../libprotobuf_arm64.a
	else
		cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make libprotobuf
		cp libprotobuf.a ../libprotobuf_armv7.a
	fi
	#lipo -create ../libSDL2_gfx_armv7.a ../libSDL2_gfx_arm64.a -output=${LIBPATH}/libSDL2_gfx.a
	;;

"zeromq")
	# Build directory
	rm -rf build-ios
	mkdir build-ios
	cd build-ios

	if [ 2 -eq $# ] && [ "$2" = "arm64" ]; then
		cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make libzmq-static
		cp lib/libzmq-static.a ../libzmq-static_arm64.a
	else
		cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZMQ_BUILD_TESTS=OFF -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}
		make libzmq-static
		cp lib/libzmq-static.a ../libzmq-static_armv7.a
	fi
	#lipo -create ../libSDL2_gfx_armv7.a ../libSDL2_gfx_arm64.a -output=${LIBPATH}/libSDL2_gfx.a
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

#../configure --host=armv7-apple-darwin --target=arm-apple-darwin --prefix=${PREFIX}

#pugixml
#cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}

#SDL2_gfx
#######../configure --host=armv7-apple-darwin --target=arm-apple-darwin --prefix=${PREFIX} --disable-mmx


#cp ../include/SDL_config_iphoneos.h ../include/SDL_config.h
#../configure --host=aarch64-apple-darwin11-lipo --target=arm-apple-darwin --prefix=${PREFIX}

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