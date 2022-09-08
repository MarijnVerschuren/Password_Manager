%module hash
%include <std_string.i>
%include <cstring.i>
%include "carrays.i"


// TODO: clean up

%typemap(in, numinputs=0) (unsigned char** buffer) (unsigned char* temp) {
  $1 = &temp;
}
%typemap(argout) (unsigned char** buffer) {
  if (*$1) {
    $result = PyBytes_FromStringAndSize((char*)*$1, 32);
    free(*$1);
  }
}

//%typemap(out) std::string %{
//    $result = PyUnicode_FromString($1.c_str());
//%}

%{
	#define SWIG_PYTHON_2_UNICODE
	// #define SWIG_PYTHON_STRICT_BYTE_CHAR
	#include "hash.hpp"
%}

%include <carrays.i>
%array_class(uint8_t, buffer);

%include "hash.hpp"