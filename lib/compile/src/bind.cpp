#include <string>

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "io.hpp"
#include "math.hpp"
#include "hash.hpp"
#include "check.hpp"
#include "encryption.hpp"



enum crc_type_enum {};  // to expose to python

PYBIND11_MODULE(py_lib, handle) {
	handle.doc() = "module for python using c++";

	// io.hpp
	handle.def("getpass", &getpass);
	handle.def("getpass", [](std::string prompt, const char replacement, bool release_gil){  // this exists so that this function can be called without halting the entire program
		if (!release_gil) { return getpass(prompt, replacement); }
		py::gil_scoped_release release;  // calling twice because the gil is only released in this scope
		return getpass(prompt, replacement);
	});

	// math.hpp


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

	// check.hpp
	py::class_<crc_type> (handle, "crc_type");
	py::enum_<crc_type_enum> crc_t(handle, "crc_t");
	crc_t.attr("crc8_ccit") =			crc8_ccit;
	crc_t.attr("crc8_rohc") =			crc8_rohc;
	crc_t.attr("crc8_cdma2000") =		crc8_cdma2000;
	crc_t.attr("crc8_wcdma") =			crc8_wcdma;
	crc_t.attr("crc8_ebu") =			crc8_ebu;
	crc_t.attr("crc8_i_code") =			crc8_i_code;
	crc_t.attr("crc8_darc") =			crc8_darc;
	crc_t.attr("crc8_dvb_s2") =			crc8_dvb_s2;
	crc_t.attr("crc8_itu") =			crc8_itu;
	crc_t.attr("crc8_maxim") =			crc8_maxim;

	crc_t.attr("crc16_ccitt") =			crc16_ccitt;
	crc_t.attr("crc16_aug_ccitt") =		crc16_aug_ccitt;
	crc_t.attr("crc16_genibus") =		crc16_genibus;
	crc_t.attr("crc16_xmodem") =		crc16_xmodem;
	crc_t.attr("crc16_mcrf4xx") =		crc16_mcrf4xx;
	crc_t.attr("crc16_riello") =		crc16_riello;
	crc_t.attr("crc16_tms37157") =		crc16_tms37157;
	crc_t.attr("crc16_a") =				crc16_a;
	crc_t.attr("crc16_kermit") =		crc16_kermit;
	crc_t.attr("crc16_x25") =			crc16_x25;
	crc_t.attr("crc16_buypass") =		crc16_buypass;
	crc_t.attr("crc16_dds_110") =		crc16_dds_110;
	crc_t.attr("crc16_arc") =			crc16_arc;
	crc_t.attr("crc16_maxim") =			crc16_maxim;
	crc_t.attr("crc16_usb") =			crc16_usb;
	crc_t.attr("crc16_modbus") =		crc16_modbus;
	crc_t.attr("crc16_dect_r") =		crc16_dect_r;
	crc_t.attr("crc16_dect_x") =		crc16_dect_x;
	crc_t.attr("crc16_en_13757") =		crc16_en_13757;
	crc_t.attr("crc16_dnp") =			crc16_dnp;
	crc_t.attr("crc16_cdma2000") =		crc16_cdma2000;
	crc_t.attr("crc16_teledisk") =		crc16_teledisk;

	crc_t.attr("crc32_bzip2") =			crc32_bzip2;
	crc_t.attr("crc32_mpeg2") =			crc32_mpeg2;
	crc_t.attr("crc32_posix") =			crc32_posix;
	crc_t.attr("crc32") =				crc32;
	crc_t.attr("crc32_jamcrc") =		crc32_jamcrc;
	crc_t.attr("crc32_q") =				crc32_q;
	crc_t.attr("crc32_xfr") =			crc32_xfr;
	crc_t.attr("crc32_c") =				crc32_c;
	crc_t.attr("crc32_d") =				crc32_d;

	crc_t.attr("crc64_ecma") =			crc64_ecma;
	crc_t.export_values();

	handle.def("init_crc8", [](crc_type type){
		uint8_t* out = init_crc8(type);
		py::bytes result = py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, 256));
		delete[] out;
		return result;
	});
	handle.def("init_crc16", [](crc_type type){
		uint16_t* out = init_crc16(type);
		py::bytes result = py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, 512));
		delete[] out;
		return result;
	});
	handle.def("init_crc32", [](crc_type type){
		uint32_t* out = init_crc32(type);
		py::bytes result = py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, 1024));
		delete[] out;
		return result;
	});
	handle.def("init_crc64", [](crc_type type){
		uint64_t* out = init_crc64(type);
		py::bytes result = py::reinterpret_steal<py::object>(PYBIND11_BYTES_FROM_STRING_AND_SIZE((char*)out, 2048));
		delete[] out;
		return result;
	});

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