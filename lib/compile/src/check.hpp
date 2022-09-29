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
struct crc_type {
	uint64_t polynomial;
	bool reflect_in;
	bool reflect_out;
	crc_bits bits;
};


// all mainstream crc algorithms
namespace crc_types {
	constexpr crc_type crc8_ccit =			{0x07,					false,	false,	bit_8};
	constexpr crc_type crc8_rohc =			{0x07,					true,	true,	bit_8};
	constexpr crc_type crc8_cdma2000 =		{0x9B,					false,	false,	bit_8};
	constexpr crc_type crc8_wcdma =			{0x9B,					true,	true,	bit_8};
	constexpr crc_type crc8_ebu =			{0x1D,					true,	true,	bit_8};
	constexpr crc_type crc8_i_code =		{0x1D,					false,	false,	bit_8};
	constexpr crc_type crc8_darc =			{0x39,					true,	true,	bit_8};
	constexpr crc_type crc8_dvb_s2 =		{0xD5,					false,	false,	bit_8};
	constexpr crc_type crc8_itu =			{0x07,					false,	false,	bit_8};
	constexpr crc_type crc8_maxim =			{0x31,					true,	true,	bit_8};

	constexpr crc_type crc16_ccitt =		{0x1021,				false,	false,	bit_16};
	constexpr crc_type crc16_aug_ccitt =	{0x1021,				false,	false,	bit_16};
	constexpr crc_type crc16_genibus =		{0x1021,				false,	false,	bit_16};
	constexpr crc_type crc16_xmodem =		{0x1021,				false,	false,	bit_16};
	constexpr crc_type crc16_mcrf4xx =		{0x1021,				true,	true,	bit_16};
	constexpr crc_type crc16_riello =		{0x1021,				true,	true,	bit_16};
	constexpr crc_type crc16_tms37157 =		{0x1021,				true,	true,	bit_16};
	constexpr crc_type crc16_a =			{0x1021,				true,	true,	bit_16};
	constexpr crc_type crc16_kermit =		{0x1021,				true,	true,	bit_16};
	constexpr crc_type crc16_x25 =			{0x1021,				true,	true,	bit_16};
	constexpr crc_type crc16_buypass =		{0x8005,				false,	false,	bit_16};
	constexpr crc_type crc16_dds_110 =		{0x8005,				false,	false,	bit_16};
	constexpr crc_type crc16_arc =			{0x8005,				true,	true,	bit_16};
	constexpr crc_type crc16_maxim =		{0x8005,				true,	true,	bit_16};
	constexpr crc_type crc16_usb =			{0x8005,				true,	true,	bit_16};
	constexpr crc_type crc16_modbus =		{0x8005,				true,	true,	bit_16};
	constexpr crc_type crc16_dect_r =		{0x0589,				false,	false,	bit_16};
	constexpr crc_type crc16_dect_x =		{0x0589,				false,	false,	bit_16};
	constexpr crc_type crc16_en_13757 =		{0x3D65,				false,	false,	bit_16};
	constexpr crc_type crc16_dnp =			{0x3D65,				true,	true,	bit_16};
	constexpr crc_type crc16_cdma2000 =		{0xC857,				false,	false,	bit_16};
	constexpr crc_type crc16_teledisk =		{0xA097,				false,	false,	bit_16};

	constexpr crc_type crc32_bzip2 =		{0x04C11DB7,			false,	false,	bit_32};
	constexpr crc_type crc32_mpeg2 =		{0x04C11DB7,			false,	false,	bit_32};
	constexpr crc_type crc32_posix =		{0x04C11DB7,			false,	false,	bit_32};
	constexpr crc_type crc32 =				{0x04C11DB7,			true,	true,	bit_32};
	constexpr crc_type crc32_jamcrc=		{0x04C11DB7,			true,	true,	bit_32};
	constexpr crc_type crc32_q =			{0x814141AB,			false,	false,	bit_32};
	constexpr crc_type crc32_xfr =			{0x000000AF,			false,	false,	bit_32};
	constexpr crc_type crc32_c =			{0x1EDC6F41,			false,	false,	bit_32};
	constexpr crc_type crc32_d =			{0xA833982B,			false,	false,	bit_32};

	constexpr crc_type crc64_ecma =			{0x42F0E1EBA9EA3693,	false,	false,	bit_64};
}

uint8_t*	init_crc8	(const uint8_t polynomial, const bool reflect_in, const bool reflect_out);
uint16_t*	init_crc16	(const uint16_t polynomial, const bool reflect_in, const bool reflect_out);
uint32_t*	init_crc32	(const uint32_t polynomial, const bool reflect_in, const bool reflect_out);
uint64_t*	init_crc64	(const uint64_t polynomial, const bool reflect_in, const bool reflect_out);

inline uint8_t*		init_crc8	(const crc_type type) { return init_crc8((uint8_t)	type.polynomial, type.reflect_in, type.reflect_out);	}
inline uint16_t*	init_crc16	(const crc_type type) { return init_crc16((uint16_t)type.polynomial, type.reflect_in, type.reflect_out);	}
inline uint32_t*	init_crc32	(const crc_type type) { return init_crc32((uint32_t)type.polynomial, type.reflect_in, type.reflect_out);	}
inline uint64_t*	init_crc64	(const crc_type type) { return init_crc64(			type.polynomial, type.reflect_in, type.reflect_out);	}

uint8_t crc_8(const void* const buffer, const uint64_t size, const uint8_t* lookup, uint8_t init = 0x00, const uint8_t xor_out = 0x00);
uint16_t crc_16(const void* const buffer, const uint64_t size, const uint16_t* lookup, uint16_t init = 0x0000, const uint16_t xor_out = 0x0000);
uint32_t crc_32(const void* const buffer, const uint64_t size, const uint32_t* lookup, uint32_t init = 0x00000000, const uint32_t xor_out = 0x00000000);
uint64_t crc_64(const void* const buffer, const uint64_t size, const uint64_t* lookup, uint64_t init = 0x0000000000000000, const uint64_t xor_out = 0x0000000000000000);


// functions for the specific types (meant for python use) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*uint8_t* crc8_ccit_table = nullptr;
inline uint8_t crc8_ccit(const void* buffer, const uint64_t size) {
	if (!crc8_ccit_table) {	crc8_ccit_table = init_crc8(types::crc8_ccit); }
	return crc_8(buffer, size, crc8_ccit_table);
}
inline void crc8_ccit_deinit() { delete[] crc8_ccit_table; crc8_ccit_table = nullptr; }
*/// TODO