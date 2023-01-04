#pragma once

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "math.hpp"

#include <iostream>


enum crc_bits {
	bit_8 = 0,
	bit_16 = 1,
	bit_32 = 2,
	bit_64 = 4,
};
struct lookup {
	bool initialized =	false;
	void* ptr = 		nullptr;
};
struct crc_type {
	uint64_t polynomial;
	lookup* table;
	bool reflect_in;
	bool reflect_out;
	uint64_t init;
	uint64_t xor_out;
	crc_bits bits;
};


// all mainstream crc algorithms
namespace crc_types {
	extern const crc_type crc8_ccit;
	extern const crc_type crc8_rohc;
	extern const crc_type crc8_cdma2000;
	extern const crc_type crc8_wcdma;
	extern const crc_type crc8_ebu;
	extern const crc_type crc8_i_code;
	extern const crc_type crc8_darc;
	extern const crc_type crc8_dvb_s2;
	extern const crc_type crc8_itu;
	extern const crc_type crc8_maxim;

	extern const crc_type crc16_ccitt;
	extern const crc_type crc16_aug_ccitt;
	extern const crc_type crc16_genibus;
	extern const crc_type crc16_xmodem;
	extern const crc_type crc16_mcrf4xx;
	extern const crc_type crc16_riello;
	extern const crc_type crc16_tms37157;
	extern const crc_type crc16_a;
	extern const crc_type crc16_kermit;
	extern const crc_type crc16_x25;
	extern const crc_type crc16_buypass;
	extern const crc_type crc16_dds_110;
	extern const crc_type crc16_arc;
	extern const crc_type crc16_maxim;
	extern const crc_type crc16_usb;
	extern const crc_type crc16_modbus;
	extern const crc_type crc16_dect_r;
	extern const crc_type crc16_dect_x;
	extern const crc_type crc16_en_13757;
	extern const crc_type crc16_dnp;
	extern const crc_type crc16_cdma2000;
	extern const crc_type crc16_teledisk;

	extern const crc_type crc32_bzip2;
	extern const crc_type crc32_mpeg2;
	extern const crc_type crc32_posix;
	extern const crc_type crc32;
	extern const crc_type crc32_jamcrc;
	extern const crc_type crc32_q;
	extern const crc_type crc32_xfr;
	extern const crc_type crc32_c;
	extern const crc_type crc32_d;

	extern const crc_type crc64_ecma;
}

// create tables (c++ side only)
uint8_t*		init_crc8	(const uint8_t polynomial, const bool reflect_in, const bool reflect_out);
uint16_t*		init_crc16	(const uint16_t polynomial, const bool reflect_in, const bool reflect_out);
uint32_t*		init_crc32	(const uint32_t polynomial, const bool reflect_in, const bool reflect_out);
uint64_t*		init_crc64	(const uint64_t polynomial, const bool reflect_in, const bool reflect_out);
// create tables (python side (managed by c++))
inline void		init_crc	(const crc_type* type) {
	if (type->table->ptr) { return; }
	switch (type->bits) {
		case crc_bits::bit_8:	type->table->ptr = init_crc8((uint8_t)type->polynomial,		type->reflect_in, type->reflect_out); return;
		case crc_bits::bit_16:	type->table->ptr = init_crc16((uint16_t)type->polynomial,	type->reflect_in, type->reflect_out); return;
		case crc_bits::bit_32:	type->table->ptr = init_crc32((uint32_t)type->polynomial,	type->reflect_in, type->reflect_out); return;
		case crc_bits::bit_64:	type->table->ptr = init_crc64(type->polynomial,				type->reflect_in, type->reflect_out); return;
		default: return;
	}
}
inline void		del_crc	(const crc_type* type) { delete[] type->table->ptr; type->table->ptr = nullptr; }

// caclculate crc using tables (c++ side only)
uint8_t			crc_8(const void* buffer, uint64_t size, const uint8_t* lookup, const uint8_t init = 0x00, const uint8_t xor_out = 0x00);
uint16_t		crc_16(const void* buffer, uint64_t size, const uint16_t* lookup, const uint16_t init = 0x0000, const uint16_t xor_out = 0x0000);
uint32_t		crc_32(const void* buffer, uint64_t size, const uint32_t* lookup, const uint32_t init = 0x00000000, const uint32_t xor_out = 0x00000000);
uint64_t		crc_64(const void* buffer, uint64_t size, const uint64_t* lookup, const uint64_t init = 0x0000000000000000, const uint64_t xor_out = 0x0000000000000000);
// caclculate crc using tables (python side (managed by c++))
inline uint64_t	crc(const void* buffer, const uint64_t size, const crc_type* type) {
	init_crc(type);
	switch (type->bits) {
	case crc_bits::bit_8:	return crc_8(buffer, size,	(uint8_t*)type->table->ptr, (uint8_t)type->init, (uint8_t)type->xor_out) &		0xff;
	case crc_bits::bit_16:	return crc_16(buffer, size,	(uint16_t*)type->table->ptr, (uint16_t)type->init, (uint16_t)type->xor_out) &	0xffff;
	case crc_bits::bit_32:	return crc_32(buffer, size,	(uint32_t*)type->table->ptr, (uint32_t)type->init, (uint32_t)type->xor_out) &	0xffffffff;
	case crc_bits::bit_64:	return crc_64(buffer, size,	(uint64_t*)type->table->ptr, type->init, type->xor_out);
	default:				return 0;
	}
}



// functions for the specific types (meant for python use) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*uint8_t* crc8_ccit_table = nullptr;
inline uint8_t crc8_ccit(const void* buffer, const uint64_t size) {
	if (!crc8_ccit_table) {	crc8_ccit_table = init_crc8(types::crc8_ccit); }
	return crc_8(buffer, size, crc8_ccit_table);
}
inline void crc8_ccit_deinit() { delete[] crc8_ccit_table; crc8_ccit_table = nullptr; }
*/// TODO