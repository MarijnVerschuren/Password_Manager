%module py_lib
%include <std_string.i>

%{
// #define SWIG_PYTHON_STRICT_BYTE_CHAR
#include "py_lib.hpp"
%}

%include "py_lib.hpp"