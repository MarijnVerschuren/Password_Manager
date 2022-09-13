%module io
%include <std_string.i>

%{
	#undef SWIG_PYTHON_STRICT_BYTE_CHAR
	#include "io.hpp"
%}

%include "io.hpp"