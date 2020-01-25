#!/usr/bin/python
# setup.py
from distutils.core import setup, Extension

setup(name="story",
	py_modules = ["story"],
	ext_modules=[
		Extension("_story",
			["scripting/SpLogHelper_wrap.cxx"],
			include_dirs=[
				'src/',
				'external/build/target/Windows/include/SDL2/',
				'external/build/source/Windows/g3log/src/',
				'external/build/source/Windows/cppzmq/',
				'external/build/source/Windows/zeromq/include/',
				'scripting/'
			],
			library_dirs=[
				'external/build/target/Windows/lib',
				'external/build/target/Windows/Debug',
				'build/Debug'
			],
			libraries=['dbghelp', 'g3logger', 'libzmq', 'libstory-project'],
			extra_compile_args=['-std=c++11', '/EHsc', '-DSWIG_PYTHON_INTERPRETER_NO_DEBUG'],
		)
	],
	package_dir={'':'scripting'},
)
