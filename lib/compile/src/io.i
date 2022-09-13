%module io
%include "cdata.i"


%typemap(in) (unsigned char* in, unsigned int inLen) {
    Py_ssize_t len;
    PyBytes_AsStringAndSize($input, &(char*)$1, &len);
    $2 = (unsigned int)len;
}
%typemap(out) (unsigned char* in, unsigned int inLen) {
	$result = (char*)$result;
}

%{
	// #define SWIG_PYTHON_STRICT_BYTE_CHAR
	#include "io.hpp"
%}


%include "io.hpp"