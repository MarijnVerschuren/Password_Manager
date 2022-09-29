#include "check.hpp"



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