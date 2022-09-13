%module encryption
%include "typemaps.i"


/*%extend AES {
	%typemap(in, numinputs=3) (const unsigned char* in, unsigned int inLen, const unsigned char* key) (char* in, unsigned int inLen, char* key) {
		$1 = (unsigned char*)in;
		$2 = inLen;
		$3 = (unsigned char*)key;
	}
	%typemap(in, numinputs=4) (const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char *iv) (char* in, unsigned int inLen, char* key, char* iv) {
		$1 = (unsigned char*)in;
		$2 = inLen;
		$3 = (unsigned char*)key;
		$4 = (unsigned char*)iv;
	}
	%typemap(in) (const unsigned char* in, unsigned int inLen) {
		if (!PyByteArray_Check($input)) {
			SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "', argument " "$argnum"" of type '" "$type""'");
		}
		$1 = (unsigned char*) PyByteArray_AsString($input);
  		$2 = (unsigned int) PyByteArray_Size($input);
	}
	/*%typemap(out) (const unsigned char* in, unsigned int inLen) {

	}
};*/


%{
	#include "encryption.hpp"
%}

%apply (char *in, unsigned int inLen, char* key) { (const unsigned char* in, unsigned int inLen, const unsigned char* key) }
%apply (char *in, unsigned int inLen, char* key, char* iv) { (const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char* iv) }

//%apply char* {unsigned char*};    
// %newobject [funcname];  // if function returns type that has to be freed

%include "encryption.hpp"