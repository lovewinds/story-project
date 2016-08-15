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
				'external/built/include/SDL2/',
				'external/sources/g3log/src/',
				'external/sources/cppzmq/',
				'external/sources/zeromq/include/',
				'scripting/'
			],
			library_dirs=[
				'external/built/lib',
				'build'
			],
			libraries=['g3logger', 'zmq-static', 'story-project'],
			extra_compile_args=['-std=c++11', '-DSWIG_PYTHON_INTERPRETER_NO_DEBUG'],
		)
	],
	package_dir={'':'scripting'},
)
