#include "math.hpp"



uint8_t bit_reverse(uint8_t value) {
	value = ((value & 0xAA) >> 1) | ((value & 0x55) << 1);
	value = ((value & 0xCC) >> 2) | ((value & 0x33) << 2);
	value = (value >> 4) | (value << 4);
	return value;
}

uint16_t bit_reverse(uint16_t value) {
	value = ((value & 0xAAAA) >> 1) | ((value & 0x5555) << 1);
	value = ((value & 0xCCCC) >> 2) | ((value & 0x3333) << 2);
	value = ((value & 0xF0F0) >> 4) | ((value & 0x0F0F) << 4);
	value = (value >> 8) | (value << 8);
	return value;
}

uint32_t bit_reverse(uint32_t value) {
	value = ((value & 0xAAAAAAAA) >> 1) | ((value & 0x55555555) << 1);
	value = ((value & 0xCCCCCCCC) >> 2) | ((value & 0x33333333) << 2);
	value = ((value & 0xF0F0F0F0) >> 4) | ((value & 0x0F0F0F0F) << 4);
	value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
	value = (value >> 16) | (value << 16);
	return value;
}

uint64_t bit_reverse(uint64_t value) {
	value = ((value & 0xAAAAAAAAAAAAAAAA) >> 1) | ((value & 0x5555555555555555) << 1);
	value = ((value & 0xCCCCCCCCCCCCCCCC) >> 2) | ((value & 0x3333333333333333) << 2);
	value = ((value & 0xF0F0F0F0F0F0F0F0) >> 4) | ((value & 0x0F0F0F0F0F0F0F0F) << 4);
	value = ((value & 0xFF00FF00FF00FF00) >> 8) | ((value & 0x00FF00FF00FF00FF) << 8);
	value = ((value & 0xFFFF0000FFFF0000) >> 16) | ((value & 0x0000FFFF0000FFFF) << 16);
	value = (value >> 32) | (value << 32);
	return value;
}