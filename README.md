Story Project
===
Cross platform game development framework based on story.


Build status
===
Build is tested with 

| [CircleCI](https://circleci.com/gh/lovewinds/story-project "CircleCI") | [Appveyor](https://ci.appveyor.com/project/lovewinds/story-project "Appveyor") |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [![CircleCI](https://circleci.com/gh/lovewinds/story-project/tree/develop.svg?style=svg)](https://circleci.com/gh/lovewinds/story-project/tree/develop) | [![Build status](https://ci.appveyor.com/api/projects/status/v0mlftorv3a5kmb6?svg=true)](https://ci.appveyor.com/project/lovewinds/story-project) |



How to build
===

Build requirements
---
**[CMake](https://cmake.org/ "CMake")**

**[Python >=3.6](https://www.python.org/ "Python >=3.6")**

Build environments
---

#### Setup build environment
Linux (Ubuntu)
```
sudo apt-get install build-essentials
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev
```

#### For external libraries


Command line build on macOS has dependency CMake and autotools,
you need to install [Brew](https://brew.sh/ "Brew") to manage above required packages easily.

##### Brew install #####
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
brew install libtool
```


Linux & Mac (command line)
---

```
mkdir build
cd build
cmake ..
make
```

Windows (Visual Studio 2015 or higher)
---

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

iOS (Xcode)
---
1. Setup environment for macOS
2. Build required external packages with these commands.
```
cd external/
python external_build.py --platform iOS
```
3. Open 'projects/xcode-ios/story-project.xcodeproj' with Xcode and build it.


#### Limitation
There is a script to build external library for iOS, but only manual build is supported now.
