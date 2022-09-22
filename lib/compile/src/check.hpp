#pragma once

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "math.hpp"



struct crc_type {
	uint64_t polynomial;
	bool reflect_in;
	bool reflect_out;
};

constexpr crc_type crc8_ccit =		{0x07,					false,	false	};
constexpr crc_type crc8_rohc =		{0x07,					true,	true	};
constexpr crc_type crc8_cdma2000 =	{0x9B,					false,	false	};
constexpr crc_type crc8_wcdma =		{0x9B,					true,	true	};
constexpr crc_type crc8_ebu =		{0x1D,					true,	true	};
constexpr crc_type crc8_i_code =	{0x1D,					false,	false	};
constexpr crc_type crc8_darc =		{0x39,					true,	true	};
constexpr crc_type crc8_dvb_s2 =	{0xD5,					false,	false	};
constexpr crc_type crc8_itu =		{0x07,					false,	false	};
constexpr crc_type crc8_maxim =		{0x31,					true,	true	};

constexpr crc_type crc16_ccitt =	{0x1021,				false,	false	};
constexpr crc_type crc16_aug_ccitt ={0x1021,				false,	false	};
constexpr crc_type crc16_genibus =	{0x1021,				false,	false	};
constexpr crc_type crc16_xmodem =	{0x1021,				false,	false	};
constexpr crc_type crc16_mcrf4xx =	{0x1021,				true,	true	};
constexpr crc_type crc16_riello =	{0x1021,				true,	true	};
constexpr crc_type crc16_tms37157 =	{0x1021,				true,	true	};
constexpr crc_type crc16_a =		{0x1021,				true,	true	};
constexpr crc_type crc16_kermit =	{0x1021,				true,	true	};
constexpr crc_type crc16_x25 =		{0x1021,				true,	true	};
constexpr crc_type crc16_buypass =	{0x8005,				false,	false	};
constexpr crc_type crc16_dds_110 =	{0x8005,				false,	false	};
constexpr crc_type crc16_arc =		{0x8005,				true,	true	};
constexpr crc_type crc16_maxim =	{0x8005,				true,	true	};
constexpr crc_type crc16_usb =		{0x8005,				true,	true	};
constexpr crc_type crc16_modbus =	{0x8005,				true,	true	};
constexpr crc_type crc16_dect_r =	{0x0589,				false,	false	};
constexpr crc_type crc16_dect_x =	{0x0589,				false,	false	};
constexpr crc_type crc16_en_13757 =	{0x3D65,				false,	false	};
constexpr crc_type crc16_dnp =		{0x3D65,				true,	true	};
constexpr crc_type crc16_cdma2000 =	{0xC857,				false,	false	};
constexpr crc_type crc16_teledisk =	{0xA097,				false,	false	};

constexpr crc_type crc32_bzip2 =	{0x04C11DB7,			false,	false	};
constexpr crc_type crc32_mpeg2 =	{0x04C11DB7,			false,	false	};
constexpr crc_type crc32_posix =	{0x04C11DB7,			false,	false	};
constexpr crc_type crc32 =			{0x04C11DB7,			true,	true	};
constexpr crc_type crc32_jamcrc=	{0x04C11DB7,			true,	true	};
constexpr crc_type crc32_q =		{0x814141AB,			false,	false	};
constexpr crc_type crc32_xfr =		{0x000000AF,			false,	false	};
constexpr crc_type crc32_c =		{0x1EDC6F41,			false,	false	};
constexpr crc_type crc32_d =		{0xA833982B,			false,	false	};

constexpr crc_type crc64_ecma =		{0x42F0E1EBA9EA3693,	false,	false	};


uint8_t*	init_crc8	(const uint8_t polynomial, const bool reflect_in, const bool reflect_out);
uint16_t*	init_crc16	(const uint16_t polynomial, const bool reflect_in, const bool reflect_out);
uint32_t*	init_crc32	(const uint32_t polynomial, const bool reflect_in, const bool reflect_out);
uint64_t*	init_crc64	(const uint64_t polynomial, const bool reflect_in, const bool reflect_out);

inline uint8_t*		init_crc8	(const crc_type type) { return init_crc8(type.polynomial, type.reflect_in, type.reflect_out);	}
inline uint16_t*	init_crc16	(const crc_type type) { return init_crc16(type.polynomial, type.reflect_in, type.reflect_out);	}
inline uint32_t*	init_crc32	(const crc_type type) { return init_crc32(type.polynomial, type.reflect_in, type.reflect_out);	}
inline uint64_t*	init_crc64	(const crc_type type) { return init_crc64(type.polynomial, type.reflect_in, type.reflect_out);	}

// https://github.com/ETLCPP/crc-table-generator