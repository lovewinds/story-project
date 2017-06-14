Story Project
===
Cross platform game development framework based on story.


Build status
===
Build is tested with [Travis CI](https://travis-ci.org/lovewinds/story-project "Travis CI")

![Travis CI](https://travis-ci.org/lovewinds/story-project.svg?branch=test)


How to build
===

Build environment
---
**[CMake](https://cmake.org/ "CMake")**
**[Python 2.X](https://www.python.org/ "Python 2.X")**
**Autotools**

Build dependencies
---


##### For external libraries


Command line build on macOS has dependency CMake and autotools,
you need to install [Brew](https://brew.sh/ "Brew") to manage above required packages easily.

######Brew install######
1. Install Xcode command line tools
```
xcode-select --install
```

2. Visit official [Brew](https://brew.sh "Brew") and get installation information like below.
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

3. Once brew is installed, setup automake, autotools via brew.
```
brew install automake
brew install autotools
```


Linux & Mac (command line)
---

```
mkdir build
cd build
cmake ..
make
```

Windows (Visual Studio 2013 or higher)
---

```
mkdir build
cd build
cmake ..
/* Build with VS or msbuild.exe */
```

iOS (Xcode)
---
1. Setup environment for macOS
2. Build required external packages with these commands.
```
cd <Project root>/external/
./build_packages.py --platform iOS
```
3. Open 'projects/xcode-ios/story-project.xcodeproj' with Xcode and build it.


#### Limitation
There is a build script for external library, but only manual build is supported now.
