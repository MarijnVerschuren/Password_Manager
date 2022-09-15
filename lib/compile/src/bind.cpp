#include <string>

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "io.hpp"
#include "hash.hpp"
#include "encryption.hpp"



PYBIND11_MODULE(py_lib, handle) {
	handle.doc() = "module for python using c++";
	// io.hpp
	handle.def("getpass", &getpass);
	// hash.hpp
	py::class_<SHA256> sha256(handle, "SHA256");
	sha256.def(py::init<>());
	sha256.def("reset", &SHA256::reset);
	sha256.def("add", py::overload_cast<const std::string&>(&SHA256::add));
	sha256.def_property_readonly("hash", &SHA256::get_hash);
	sha256.def_property_readonly("raw_hash", [](SHA256* self){
		unsigned char* data;
		self->get_raw_hash(&data);
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)data, 32));
	});
	sha256.def("__call__", py::overload_cast<const std::string&>(&SHA256::operator()));

	// encryption.hpp
	
}

// TODO: return raw hash and pass raw data to add function
// add operator() support

// encryption


/*  // NOT NEEDED
sha256.def("add", [](SHA256* self, py::bytes data){
	char* buffer; ssize_t len;
	if(PyBytes_AsStringAndSize(data.ptr(), &buffer, &len)) { return; }  // fail
	self->add(buffer, len);
});

sha256.def("__call__", [](SHA256* self, py::bytes data){
	char* buffer; ssize_t len;
	if(PyBytes_AsStringAndSize(data.ptr(), &buffer, &len)) { return std::string(64, '0'); }  // fail
	return (*self)(buffer, len);
});
*/