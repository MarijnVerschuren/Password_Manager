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
	py::class_<AES> aes(handle, "AES");
	aes.def(py::init<const AES_key>());
	// TODO: custom conversion from string/bytes to vector
	aes.def("encrypt_ECB", py::overload_cast<std::vector<unsigned char>, std::vector<unsigned char>>(&AES::encrypt_ECB));
	aes.def("decrypt_ECB", py::overload_cast<std::vector<unsigned char>, std::vector<unsigned char>>(&AES::decrypt_ECB));
	aes.def("encrypt_CBC", py::overload_cast<std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>>(&AES::encrypt_CBC));
	aes.def("decrypt_CBC", py::overload_cast<std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>>(&AES::decrypt_CBC));
	aes.def("encrypt_CFB", py::overload_cast<std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>>(&AES::encrypt_CFB));
	aes.def("decrypt_CFB", py::overload_cast<std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>>(&AES::decrypt_CFB));
	aes.def("hex_vector", &AES::print_hex_vector);

	py::enum_<AES_key> aes_key(aes, "AES_key");
    aes_key.value("AES_128", AES_key::AES_128);
    aes_key.value("AES_192", AES_key::AES_192);
    aes_key.value("AES_256", AES_key::AES_256);
    aes_key.export_values();
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