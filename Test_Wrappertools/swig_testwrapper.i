/*module name*/

%module testwrapper

%{
#define SWIG_FILE_WITH_INIT
#include "testwrapper.h"
%}

%include "testwrapper.h"
