#pragma once

#include <iostream>
#include <string>
#include <windows.h>

#define int8	char
#define uint8	unsigned char
#define int16	short
#define uint16	unsigned short
#define int32	long
#define uint32	unsigned long
#define int64	long long
#define uint64	unsigned long long

#define uint	unsigned int

#define f32		float
#define f64		double

#define CON_BACKSPACE 8
#define CON_RETURN 13  // Carriage Return



std::string getpass(const char* prompt, bool show_asterisk = true);