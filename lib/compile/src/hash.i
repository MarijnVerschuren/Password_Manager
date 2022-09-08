%module hash
%include <std_string.i>

%{
// #define SWIG_PYTHON_STRICT_BYTE_CHAR
#include "hash.hpp"
%}

%include "hash.hpp"