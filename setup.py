#!/usr/bin/python
# setup.py
import os
from distutils.core import setup, Extension

PLATFORM=os.getenv('_PLATFORM')
if PLATFORM is None:
	PLATFORM='Linux'
print("[PLATFORM]: {}".format(PLATFORM))

setup(name="story",
	py_modules = ["story"],
	ext_modules=[
		Extension("_story",
			["scripting/SpLogHelper_wrap.cxx"],
			include_dirs=[
				'include/',
				'external/lib_build/{}/include/SDL2/'.format(PLATFORM),
				'external/lib_source/{}/g3log/src/'.format(PLATFORM),
				'external/lib_source/{}/cppzmq/'.format(PLATFORM),
				'external/lib_source/{}/zeromq/include/'.format(PLATFORM),
				'scripting/'
			],
			library_dirs=[
				'external/lib_build/{}/lib'.format(PLATFORM),
				'build'
			],
			libraries=['g3logger', 'zmq-static', 'story-project'],
			extra_compile_args=['-std=c++11', '-DSWIG_PYTHON_INTERPRETER_NO_DEBUG'],
		)
	],
	package_dir={'':'scripting'},
)
