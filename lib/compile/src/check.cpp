#include "check.hpp"



lookup crc8_table_0;
lookup crc8_table_1;
lookup crc8_table_2;
lookup crc8_table_3;
lookup crc8_table_4;
lookup crc8_table_5;
lookup crc8_table_6;
lookup crc8_table_7;
lookup crc8_table_8;

lookup crc16_table_0;
lookup crc16_table_1;
lookup crc16_table_2;
lookup crc16_table_3;
lookup crc16_table_4;
lookup crc16_table_5;
lookup crc16_table_6;
lookup crc16_table_7;
lookup crc16_table_8;

lookup crc32_table_0;
lookup crc32_table_1;
lookup crc32_table_2;
lookup crc32_table_3;
lookup crc32_table_4;
lookup crc32_table_5;

lookup crc64_table;


const crc_type crc_types::crc8_ccit =			{0x07,					&crc8_table_0,				false,	false,	bit_8};		// 0
const crc_type crc_types::crc8_rohc =			{0x07,					&crc8_table_1,				true,	true,	bit_8};
const crc_type crc_types::crc8_cdma2000 =		{0x9B,					&crc8_table_2,				false,	false,	bit_8};
const crc_type crc_types::crc8_wcdma =			{0x9B,					&crc8_table_3,				true,	true,	bit_8};
const crc_type crc_types::crc8_ebu =			{0x1D,					&crc8_table_4,				true,	true,	bit_8};
const crc_type crc_types::crc8_i_code =			{0x1D,					&crc8_table_5,				false,	false,	bit_8};
const crc_type crc_types::crc8_darc =			{0x39,					&crc8_table_6,				true,	true,	bit_8};
const crc_type crc_types::crc8_dvb_s2 =			{0xD5,					&crc8_table_7,				false,	false,	bit_8};
const crc_type crc_types::crc8_itu =			{0x07,					&crc8_table_0,				false,	false,	bit_8};		// 0
const crc_type crc_types::crc8_maxim =			{0x31,					&crc8_table_8,				true,	true,	bit_8};
const crc_type crc_types::crc16_ccitt =			{0x1021,				&crc16_table_0,				false,	false,	bit_16};	// 0
const crc_type crc_types::crc16_aug_ccitt =		{0x1021,				&crc16_table_0,				false,	false,	bit_16};	// 0
const crc_type crc_types::crc16_genibus =		{0x1021,				&crc16_table_0,				false,	false,	bit_16};	// 0
const crc_type crc_types::crc16_xmodem =		{0x1021,				&crc16_table_0,				false,	false,	bit_16};	// 0
const crc_type crc_types::crc16_mcrf4xx =		{0x1021,				&crc16_table_1,				true,	true,	bit_16};	// 1
const crc_type crc_types::crc16_riello =		{0x1021,				&crc16_table_1,				true,	true,	bit_16};	// 1
const crc_type crc_types::crc16_tms37157 =		{0x1021,				&crc16_table_1,				true,	true,	bit_16};	// 1
const crc_type crc_types::crc16_a =				{0x1021,				&crc16_table_1,				true,	true,	bit_16};	// 1
const crc_type crc_types::crc16_kermit =		{0x1021,				&crc16_table_1,				true,	true,	bit_16};	// 1
const crc_type crc_types::crc16_x25 =			{0x1021,				&crc16_table_1,				true,	true,	bit_16};	// 1
const crc_type crc_types::crc16_buypass =		{0x8005,				&crc16_table_2,				false,	false,	bit_16};	// 2
const crc_type crc_types::crc16_dds_110 =		{0x8005,				&crc16_table_2,				false,	false,	bit_16};	// 2
const crc_type crc_types::crc16_arc =			{0x8005,				&crc16_table_3,				true,	true,	bit_16};	// 3
const crc_type crc_types::crc16_maxim =			{0x8005,				&crc16_table_3,				true,	true,	bit_16};	// 3
const crc_type crc_types::crc16_usb =			{0x8005,				&crc16_table_3,				true,	true,	bit_16};	// 3
const crc_type crc_types::crc16_modbus =		{0x8005,				&crc16_table_3,				true,	true,	bit_16};	// 3
const crc_type crc_types::crc16_dect_r =		{0x0589,				&crc16_table_4,				false,	false,	bit_16};	// 4
const crc_type crc_types::crc16_dect_x =		{0x0589,				&crc16_table_4,				false,	false,	bit_16};	// 4
const crc_type crc_types::crc16_en_13757 =		{0x3D65,				&crc16_table_5,				false,	false,	bit_16};
const crc_type crc_types::crc16_dnp =			{0x3D65,				&crc16_table_6,				true,	true,	bit_16};
const crc_type crc_types::crc16_cdma2000 =		{0xC857,				&crc16_table_7,				false,	false,	bit_16};
const crc_type crc_types::crc16_teledisk =		{0xA097,				&crc16_table_8,				false,	false,	bit_16};
const crc_type crc_types::crc32_bzip2 =			{0x04C11DB7,			&crc32_table_0,				false,	false,	bit_32};	// 0
const crc_type crc_types::crc32_mpeg2 =			{0x04C11DB7,			&crc32_table_0,				false,	false,	bit_32};	// 0
const crc_type crc_types::crc32_posix =			{0x04C11DB7,			&crc32_table_0,				false,	false,	bit_32};	// 0
const crc_type crc_types::crc32 =				{0x04C11DB7,			&crc32_table_1,				true,	true,	bit_32};	// 1
const crc_type crc_types::crc32_jamcrc=			{0x04C11DB7,			&crc32_table_1,				true,	true,	bit_32};	// 1
const crc_type crc_types::crc32_q =				{0x814141AB,			&crc32_table_2,				false,	false,	bit_32};
const crc_type crc_types::crc32_xfr =			{0x000000AF,			&crc32_table_3,				false,	false,	bit_32};
const crc_type crc_types::crc32_c =				{0x1EDC6F41,			&crc32_table_4,				false,	false,	bit_32};
const crc_type crc_types::crc32_d =				{0xA833982B,			&crc32_table_5,				false,	false,	bit_32};
const crc_type crc_types::crc64_ecma =			{0x42F0E1EBA9EA3693,	&crc64_table,				false,	false,	bit_64};



uint8_t* init_crc8(const uint8_t polynomial, const bool reflect_in, const bool reflect_out) {
	uint8_t* out = new uint8_t[256];
	for (uint8_t byte = 0;; byte++) {
		uint8_t crc = (reflect_in ? bit_reverse((uint8_t)byte) : byte);
		for (uint8_t bit = 0; bit < 8; bit++) {
			if (crc & 0x80) {
				crc = (crc << 1) ^ polynomial;
				continue;
			}
			crc <<= 1;
		}
		out[byte] = (reflect_out ? bit_reverse(crc) : crc);
		if (byte == 0xff) { break; }  // to add 0xff
	}
	return out;
}
uint16_t* init_crc16(const uint16_t polynomial, const bool reflect_in, const bool reflect_out) {
	uint16_t* out = new uint16_t[256];
	for (uint8_t byte = 0;; byte++) {
		uint16_t crc = (reflect_in ? (bit_reverse((uint16_t)byte) >> 8) : byte);
		for (uint8_t bit = 0; bit < 16; bit++) {
			if (crc & 0x8000) {
				crc = (crc << 1) ^ polynomial;
				continue;
			}
			crc <<= 1;
		}
		out[byte] = (reflect_out ? bit_reverse(crc) : crc);
		if (byte == 0xff) { break; }  // to add 0xff
	}
	return out;
}
uint32_t* init_crc32(const uint32_t polynomial, const bool reflect_in, const bool reflect_out) {
	uint32_t* out = new uint32_t[256];
	for (uint8_t byte = 0;; byte++) {
		uint32_t crc = (reflect_in ? (bit_reverse((uint32_t)byte) >> 24) : byte);
		for (uint8_t bit = 0; bit < 32; bit++) {
			if (crc & 0x80000000) {
				crc = (crc << 1) ^ polynomial;
				continue;
			}
			crc <<= 1;
		}
		out[byte] = (reflect_out ? bit_reverse(crc) : crc);
		if (byte == 0xff) { break; }  // to add 0xff
	}

	return out;
}
uint64_t* init_crc64(const uint64_t polynomial, const bool reflect_in, const bool reflect_out) {
	uint64_t* out = new uint64_t[256];
	for (uint8_t byte = 0;; byte++) {
		uint64_t crc = (reflect_in ? (bit_reverse((uint64_t)byte) >> 56) : byte);
		for (uint8_t bit = 0; bit < 64; bit++) {
			if (crc & 0x8000000000000000) {
				crc = (crc << 1) ^ polynomial;
				continue;
			}
			crc <<= 1;
		}
		out[byte] = (reflect_out ? bit_reverse(crc) : crc);
		if (byte == 0xff) { break; }  // to add 0xff
	}
	return out;
}


uint64_t crc_64(const void* const buffer, const uint64_t size, const uint64_t* lookup, uint64_t init, const uint64_t xor_out) {
	for (uint64_t i = 0; i < size; i++) { init = (init << 8) ^ lookup[((init >> 8) ^ ((const uint8_t*)buffer)[i]) & 0xff]; }
	return init ^ xor_out;
}
uint32_t crc_32(const void* const buffer, const uint64_t size, const uint32_t* lookup, uint32_t init, const uint32_t xor_out) {
	for (uint64_t i = 0; i < size; i++) { init = (init << 8) ^ lookup[((init >> 8) ^ ((const uint8_t*)buffer)[i]) & 0xff]; }
	return init ^ xor_out;
}
uint16_t crc_16(const void* const buffer, const uint64_t size, const uint16_t* lookup, uint16_t init, const uint16_t xor_out) {
	for (uint64_t i = 0; i < size; i++) { init = (init << 8) ^ lookup[((init >> 8) ^ ((const uint8_t*)buffer)[i]) & 0xff]; }
	return init ^ xor_out;
}
uint8_t crc_8(const void* const buffer, const uint64_t size, const uint8_t* lookup, uint8_t init, const uint8_t xor_out) {
	for (uint64_t i = 0; i < size; i++) { init = (init << 8) ^ lookup[((init >> 8) ^ ((const uint8_t*)buffer)[i]) & 0xff]; }
	return init ^ xor_out;
}