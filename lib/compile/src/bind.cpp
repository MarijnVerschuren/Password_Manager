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

	// hash.hpp
	py::class_<SHA256> sha256(handle, "SHA256");
	sha256.def(py::init<>());
	sha256.def("reset", &SHA256::reset);
	sha256.def("add", py::overload_cast<const std::string&>(&SHA256::add));
	sha256.def_property_readonly("hash", &SHA256::get_hash);
	sha256.def_property_readonly("raw_hash", [](SHA256* self){
		unsigned char* data;
		self->get_raw_hash(&data);
		py::bytes result = py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)data, 32));
		delete[] data;
		return result;
	});
	sha256.def("__call__", py::overload_cast<const std::string&>(&SHA256::operator()));

	py::class_<SHA3> sha3(handle, "SHA3");
	py::enum_<SHA3::SHA3_type> sha3_type(sha3, "SHA3_type");
    sha3_type.value("bits224", SHA3::SHA3_type::bits224);
    sha3_type.value("bits256", SHA3::SHA3_type::bits256);
    sha3_type.value("bits384", SHA3::SHA3_type::bits384);
    sha3_type.value("bits512", SHA3::SHA3_type::bits512);
    sha3_type.export_values();

	sha3.def(py::init<SHA3::SHA3_type>());
	sha3.def("reset", &SHA3::reset);
	sha3.def("add", py::overload_cast<const std::string&>(&SHA3::add));
	sha3.def_property_readonly("hash", &SHA3::get_hash);
	sha3.def_property_readonly("raw_hash", [](SHA3* self){
		unsigned char* data;
		self->get_raw_hash(&data);
		py::bytes result = py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)data, self->m_bits / 8));
		delete[] data;
		return result;
	});
	sha3.def("__call__", py::overload_cast<const std::string&>(&SHA3::operator()));

	// encryption.hpp
	py::class_<AES> aes(handle, "AES");
	py::enum_<AES_key_type> aes_key(aes, "AES_key_type");
	aes_key.value("AES_128", AES_key_type::AES_128);
	aes_key.value("AES_192", AES_key_type::AES_192);
	aes_key.value("AES_256", AES_key_type::AES_256);
	aes_key.export_values();

	aes.def(py::init<const AES_key_type>());
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
}