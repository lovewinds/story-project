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
				'external/lib_build/Windows/include/SDL2/',
				'external/lib_source/Windows/g3log/src/',
				'external/lib_source/Windows/cppzmq/',
				'external/lib_source/Windows/zeromq/include/',
				'scripting/'
			],
			library_dirs=[
				'external/lib_build/Windows/lib',
				'external/lib_build/Windows/Debug',
				'build/Debug'
			],
			libraries=['dbghelp', 'g3logger', 'libzmq', 'libstory-project'],
			extra_compile_args=['-std=c++11', '/EHsc', '-DSWIG_PYTHON_INTERPRETER_NO_DEBUG'],
		)
	],
	package_dir={'':'scripting'},
)
