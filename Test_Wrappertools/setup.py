#!/usr/bin/env python3
#cython: language_level=3

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension("cython_wrapper",
                             ["cython_testwrapper.pyx", "testwrapper.cpp"], language="c++",
                    extra_compile_args = ['-std=c++17'])],
      cmdclass = {'build_ext': build_ext})
