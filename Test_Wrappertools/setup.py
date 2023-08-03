# File : setup.py

from distutils.core import setup, Extension
#name of module
name = "Test_Wrapper"

#version of module
version = "1.0"

# specify the name of the extension and source files
# required to compile this
ext_modules = Extension(name='_Test_Wrapper',sources=["swig_testwrapper.i","testwrapper.cpp"])

setup(name=name,
	version=version,
	ext_modules=[ext_modules])

