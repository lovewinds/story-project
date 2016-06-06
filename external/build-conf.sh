export SRC="~/Documents/source/github/story-project"
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
#export AR=$(xcrun -sdk iphoneos -find ar)
#export RANLIB=$(xcrun -sdk iphoneos -find ranlib)

#armv7
export CFLAGS="${CFLAGS} -arch armv7 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
export CXXFLAGS="${CXXFLAGS} -arch armv7 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
export CPPFLAGS="${CPPFLAGS} -arch armv7 -isysroot ${SDK} -mios-version-min=${IOS_VERSION_MIN} -fembed-bitcode"
export LDFLAGS="${LDFLAGS} -L${LIBPATH} -isysroot ${SDK} -F${FRAMEWORKS} -framework Foundation -framework UIKit -framework OpenGLES -framework QuartzCore -framework CoreMotion -framework GameController -framework CoreAudio -framework AudioToolbox -framework CoreGraphics -framework ImageIO -fembed-bitcode"

#SDL2_gfx
./autogen.sh

#rm -rf build32
#mkdir build32
#cd build32

rm -rf build32
mkdir build32
cd build32

#../configure --host=armv7-apple-darwin --target=arm-apple-darwin --prefix=${PREFIX}

#pugixml
#cmake .. -DCMAKE_INSTALL_LIBDIR=${LIBPATH} -DCMAKE_INSTALL_INCLUDEDIR=${INCLUDEPATH}

#SDL2_gfx
../configure --host=armv7-apple-darwin --target=arm-apple-darwin --prefix=${PREFIX} --disable-mmx


#cp ../include/SDL_config_iphoneos.h ../include/SDL_config.h
#../configure --host=aarch64-apple-darwin11-lipo --target=arm-apple-darwin --prefix=${PREFIX}

make
#make install


#xcodebuild -arch arm64 -arch armv7

# SDL2
#xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS=${INCLUDEPATH}/SDL2/

# SDL2_image
#xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS=${INCLUDEPATH}/SDL2/:${INCLUDEPATH}

# SDL2_ttf
#xcodebuild -arch arm64 -arch armv7 HEADER_SEARCH_PATHS_QUOTED_FOR_PROJECT_1=${INCLUDEPATH}/SDL2/