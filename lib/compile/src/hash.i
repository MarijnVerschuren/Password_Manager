%module hash
%include <std_string.i>
%include <cstring.i>
%include "carrays.i"


// TODO: clean up

%typemap(in, numinputs=0) (unsigned char** buffer) (unsigned char* temp) {
  $1 = &temp;
}

// only apply typemap to SHA256 class (CHECK IF THIS WORKS) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
%extend SHA256 {
  %typemap(argout) (unsigned char** buffer) {
    if (*$1) {
      $result = PyBytes_FromStringAndSize((char*)*$1, 32);
      free(*$1);
    }
    else { $result = Py_None; }
  }
};
// https://stackoverflow.com/questions/69905004/how-do-i-apply-a-swig-typemap-to-only-a-specific-function


%{
	#include "hash.hpp"
%}

%include <carrays.i>
%array_class(uint8_t, buffer);

%include "hash.hpp"
