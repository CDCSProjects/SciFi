/*module name*/

%module swig_wrapper
%include "std_string.i"
%{
#define SWIG_FILE_WITH_INIT
#include "testwrapper.h"
%}

%include "testwrapper.h"
