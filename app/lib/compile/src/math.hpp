#pragma once

#include "pybind11/pybind11.h"
namespace py = pybind11;



uint8_t bit_reverse(uint8_t value);
uint16_t bit_reverse(uint16_t value);
uint32_t bit_reverse(uint32_t value);
uint64_t bit_reverse(uint64_t value);