#pragma once

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include <iostream>
#include <string>
#include <windows.h>
#include <stdint.h>
#include <stdio.h>

#define CON_BACKSPACE 8
#define CON_RETURN 13  // Carriage Return

constexpr const char HEX_CHARS[17] = "0123456789abcdef";  // (+0x00)

std::string getpass(std::string prompt, const char replacement = '*');

// TODO: change print hex array to a set of functions that process all number types