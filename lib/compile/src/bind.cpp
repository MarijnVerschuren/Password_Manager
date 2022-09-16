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
	handle.def("print_hex_array", [](std::string& data){ print_hex_array((unsigned char*)data.c_str(), data.length()); });

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
	aes.def("encrypt_ECB", [](AES* self, std::string& data, std::string& key){
		unsigned char* out = self->encrypt_ECB((unsigned char*)data.c_str(), (unsigned int)data.length(), (unsigned char*)key.c_str());
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, (unsigned int)data.length()));
	});
	aes.def("decrypt_ECB", [](AES* self, std::string& data, std::string& key){
		unsigned char* out = self->decrypt_ECB((unsigned char*)data.c_str(), (unsigned int)data.length(), (unsigned char*)key.c_str());
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, (unsigned int)data.length()));
	});
	aes.def("encrypt_CBC", [](AES* self, std::string& data, std::string& key, std::string& iv){
		unsigned char* out = self->encrypt_CBC((unsigned char*)data.c_str(), (unsigned int)data.length(), (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, (unsigned int)data.length()));
	});
	aes.def("decrypt_CBC", [](AES* self, std::string& data, std::string& key, std::string& iv){
		unsigned char* out = self->decrypt_CBC((unsigned char*)data.c_str(), (unsigned int)data.length(), (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, (unsigned int)data.length()));
	});
	aes.def("encrypt_CFB", [](AES* self, std::string& data, std::string& key, std::string& iv){
		unsigned char* out = self->encrypt_CFB((unsigned char*)data.c_str(), (unsigned int)data.length(), (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, (unsigned int)data.length()));
	});
	aes.def("decrypt_CFB", [](AES* self, std::string& data, std::string& key, std::string& iv){
		unsigned char* out = self->decrypt_CFB((unsigned char*)data.c_str(), (unsigned int)data.length(), (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
		return py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, (unsigned int)data.length()));
	});

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
aes.def("print_hex_array", [](AES* self, std::string& data){
		self->print_hex_array((unsigned char*)data.c_str(), data.length());
	});

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