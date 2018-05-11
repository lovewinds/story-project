#!/usr/bin/python
# setup.py
from distutils.core import setup, Extension

setup(name="story",
	py_modules = ["story"],
	ext_modules=[
		Extension("_story",
			["scripting/SpLogHelper_wrap.cxx"],
			include_dirs=[
				'include/',
				'external/lib_build/macOS/include/SDL2/',
				'external/lib_source/macOS/g3log/src/',
				'external/lib_source/macOS/cppzmq/',
				'external/lib_source/macOS/zeromq/include/',
				'scripting/'
			],
			library_dirs=[
				'external/lib_build/macOS/lib',
				'build'
			],
			libraries=['g3logger', 'zmq-static', 'story-project'],
			extra_compile_args=['-std=c++11', '-DSWIG_PYTHON_INTERPRETER_NO_DEBUG'],
		)
	],
	package_dir={'':'scripting'},
)
