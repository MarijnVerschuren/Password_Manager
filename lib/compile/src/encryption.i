%module encryption
%include <std_string.i>

%{
// #define SWIG_PYTHON_STRICT_BYTE_CHAR
#include "encryption.hpp"
%}

%include "encryption.hpp"