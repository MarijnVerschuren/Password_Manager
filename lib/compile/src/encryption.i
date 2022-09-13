%module encryption
%include "typemaps.i"
%include <std_string.i>


%extend AES {
	/*%typemap(in, numinputs=3) (const unsigned char* in, unsigned int inLen, const unsigned char* key) (std::string in, std::string key) {
		$1 = (unsigned char*)in.c_str();
		$2 = in.len();
		$3 = (unsigned char*)key.c_str();
	}
	%typemap(in, numinputs=4) (const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char *iv) (std::string in, std::string key, std::string iv) {
		$1 = (unsigned char*)in.c_str();
		$2 = in.len();
		$3 = (unsigned char*)key.c_str();
		$4 = (unsigned char*)iv.c_str();
	}*/
	/*%typemap(in) (const unsigned char* in, unsigned int inLen) {
		if (!PyByteArray_Check($input)) {
			SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "', argument " "$argnum"" of type '" "$type""'");
		}
		$1 = (unsigned char*) PyByteArray_AsString($input);
  		$2 = (unsigned int) PyByteArray_Size($input);
	}
	%typemap(out) (const unsigned char* in, unsigned int inLen) {

	}*/
};


%{
	#include "encryption.hpp"
%}

//%apply char* {unsigned char*};    
// %newobject [funcname];  // if function returns type that has to be freed

%include "encryption.hpp"