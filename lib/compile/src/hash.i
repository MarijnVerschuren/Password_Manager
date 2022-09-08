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



/*  See if this can be inplemented
%{
const char* getName(int arg1, int arg2) {
    static char s[] {"something"};
    return s;
}

const char* returnBinaryData(int arg1, int arg2, size_t ret_length) {
    auto s = new char[ret_length];
    for(size_t i = 0; i < ret_length; ++i)
        s[i] = static_cast<char>(i % 256);
    return s;
}

// wrapper to allow custom typemap
void mygetbin(int arg1, int arg2, size_t length, const char** output) {
    *output = returnBinaryData(arg1, arg2, length);
}
%}

%include <exception.i>

// Input typemap that expects a Python integer input for this precise pair of parameters
%typemap(in) (size_t length, const char** output) (char* out = nullptr) %{
    $2 = &out;
    if(!PyLong_Check($input))
        SWIG_exception(SWIG_TypeError, "expected integer");
    $1 = PyLong_AsUnsignedLongLong($input);
%}

// A Python bytes object will be returned that Python will manage,
// so free the C++ allocation to prevent a memory leak.
%typemap(freearg) (size_t length, const char** output) %{
    delete[] *$2;
%}

// Append the output argument to the return value.
%typemap(argout) (size_t length, const char** output) %{
    $result = SWIG_Python_AppendOutput($result, PyBytes_FromStringAndSize(*$2, $1));
%}

%ignore returnBinaryData; // necessary if getName and returnBinaryData included via %include "someheader.h"

// Process this function normally with default handling of char* output.
// This could be %include "someheader.h" instead, and all the functions would be swigged except
// for the ignored function.
const char* getName(int arg1, int arg2);

// Explicitly rename the wrapper and process it.
%rename(returnBinaryData) mygetbin;
void mygetbin(int arg1, int arg2, size_t length, const char** output);
*/

// https://stackoverflow.com/questions/69905004/how-do-i-apply-a-swig-typemap-to-only-a-specific-function