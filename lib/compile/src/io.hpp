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


std::string getpass(std::string prompt, const char replacement = '*');