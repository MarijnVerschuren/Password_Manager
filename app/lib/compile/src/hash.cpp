#include "hash.hpp"



SHA256::SHA256() {
	reset();
}

/// restart
void SHA256::reset() {
	m_numBytes   = 0;
	m_bufferSize = 0;

	// according to RFC 1321
	// "These words were obtained by taking the first thirty-two bits of the
	//  fractional parts of the square roots of the first eight prime numbers"
	m_hash[0] = 0x6a09e667;
	m_hash[1] = 0xbb67ae85;
	m_hash[2] = 0x3c6ef372;
	m_hash[3] = 0xa54ff53a;
	m_hash[4] = 0x510e527f;
	m_hash[5] = 0x9b05688c;
	m_hash[6] = 0x1f83d9ab;
	m_hash[7] = 0x5be0cd19;

#ifdef SHA2_224_SEED_VECTOR
	// if you want SHA2-224 instead then use these seeds
	// and throw away the last 32 bits of get_hash
	m_hash[0] = 0xc1059ed8;
	m_hash[1] = 0x367cd507;
	m_hash[2] = 0x3070dd17;
	m_hash[3] = 0xf70e5939;
	m_hash[4] = 0xffc00b31;
	m_hash[5] = 0x68581511;
	m_hash[6] = 0x64f98fa7;
	m_hash[7] = 0xbefa4fa4;
#endif
}

namespace {
	inline uint32_t rotate(uint32_t a, uint32_t c) {
		return (a >> c) | (a << (32 - c));
	}

	inline uint32_t swap(uint32_t x) {
#if defined(__GNUC__) || defined(__clang__)
		return __builtin_bswap32(x);
#endif
#ifdef MSC_VER
		return _byteswap_ulong(x);
#endif
		return (x >> 24) | ((x >>  8) & 0x0000FF00) | ((x <<  8) & 0x00FF0000) | (x << 24);
	}

	// mix functions for process_block()
	inline uint32_t f1(uint32_t e, uint32_t f, uint32_t g) {
		uint32_t term1 = rotate(e, 6) ^ rotate(e, 11) ^ rotate(e, 25);
		uint32_t term2 = (e & f) ^ (~e & g); //(g ^ (e & (f ^ g)))
		return term1 + term2;
	}

	inline uint32_t f2(uint32_t a, uint32_t b, uint32_t c) {
		uint32_t term1 = rotate(a, 2) ^ rotate(a, 13) ^ rotate(a, 22);
		uint32_t term2 = ((a | b) & c) | (a & b); //(a & (b ^ c)) ^ (b & c);
		return term1 + term2;
	}
}

/// process 64 bytes
void SHA256::process_block(const void* data) {
	// get last hash
	uint32_t a = m_hash[0];
	uint32_t b = m_hash[1];
	uint32_t c = m_hash[2];
	uint32_t d = m_hash[3];
	uint32_t e = m_hash[4];
	uint32_t f = m_hash[5];
	uint32_t g = m_hash[6];
	uint32_t h = m_hash[7];

	// data represented as 16x 32-bit words
	const uint32_t* input = (uint32_t*) data;
	// convert to big endian
	uint32_t words[64];
	int i;
	for (i = 0; i < 16; i++) {
#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
		words[i] =      input[i];
#else
		words[i] = swap(input[i]);
#endif
	}

	uint32_t x,y; // temporaries

	// first round
	x = h + f1(e,f,g) + 0x428a2f98 + words[ 0]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0x71374491 + words[ 1]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0xb5c0fbcf + words[ 2]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0xe9b5dba5 + words[ 3]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0x3956c25b + words[ 4]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0x59f111f1 + words[ 5]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0x923f82a4 + words[ 6]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0xab1c5ed5 + words[ 7]; y = f2(b,c,d); e += x; a = x + y;

	// secound round
	x = h + f1(e,f,g) + 0xd807aa98 + words[ 8]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0x12835b01 + words[ 9]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0x243185be + words[10]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0x550c7dc3 + words[11]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0x72be5d74 + words[12]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0x80deb1fe + words[13]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0x9bdc06a7 + words[14]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0xc19bf174 + words[15]; y = f2(b,c,d); e += x; a = x + y;

	// extend to 24 words
	for (; i < 24; i++) {
		words[i] = words[i-16] + (rotate(words[i-15],  7) ^ rotate(words[i-15], 18) ^ (words[i-15] >>  3)) + words[i-7] + (rotate(words[i- 2], 17) ^ rotate(words[i- 2], 19) ^ (words[i- 2] >> 10));
	}

	// third round
	x = h + f1(e,f,g) + 0xe49b69c1 + words[16]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0xefbe4786 + words[17]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0x0fc19dc6 + words[18]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0x240ca1cc + words[19]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0x2de92c6f + words[20]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0x4a7484aa + words[21]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0x5cb0a9dc + words[22]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0x76f988da + words[23]; y = f2(b,c,d); e += x; a = x + y;

	// extend to 32 words
	for (; i < 32; i++) {
		words[i] = words[i-16] + (rotate(words[i-15],  7) ^ rotate(words[i-15], 18) ^ (words[i-15] >>  3)) + words[i-7] + (rotate(words[i- 2], 17) ^ rotate(words[i- 2], 19) ^ (words[i- 2] >> 10));
	}

	// fourth round
	x = h + f1(e,f,g) + 0x983e5152 + words[24]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0xa831c66d + words[25]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0xb00327c8 + words[26]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0xbf597fc7 + words[27]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0xc6e00bf3 + words[28]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0xd5a79147 + words[29]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0x06ca6351 + words[30]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0x14292967 + words[31]; y = f2(b,c,d); e += x; a = x + y;

	// extend to 40 words
	for (; i < 40; i++) {
		words[i] = words[i-16] + (rotate(words[i-15],  7) ^ rotate(words[i-15], 18) ^ (words[i-15] >>  3)) + words[i-7] + (rotate(words[i- 2], 17) ^ rotate(words[i- 2], 19) ^ (words[i- 2] >> 10));
	}

	// fifth round
	x = h + f1(e,f,g) + 0x27b70a85 + words[32]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0x2e1b2138 + words[33]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0x4d2c6dfc + words[34]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0x53380d13 + words[35]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0x650a7354 + words[36]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0x766a0abb + words[37]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0x81c2c92e + words[38]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0x92722c85 + words[39]; y = f2(b,c,d); e += x; a = x + y;

	// extend to 48 words
	for (; i < 48; i++) {
		words[i] = words[i-16] + (rotate(words[i-15],  7) ^ rotate(words[i-15], 18) ^ (words[i-15] >>  3)) + words[i-7] + (rotate(words[i- 2], 17) ^ rotate(words[i- 2], 19) ^ (words[i- 2] >> 10));
	}

	// sixth round
	x = h + f1(e,f,g) + 0xa2bfe8a1 + words[40]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0xa81a664b + words[41]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0xc24b8b70 + words[42]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0xc76c51a3 + words[43]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0xd192e819 + words[44]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0xd6990624 + words[45]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0xf40e3585 + words[46]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0x106aa070 + words[47]; y = f2(b,c,d); e += x; a = x + y;

	// extend to 56 words
	for (; i < 56; i++) {
		words[i] = words[i-16] + (rotate(words[i-15],  7) ^ rotate(words[i-15], 18) ^ (words[i-15] >>  3)) + words[i-7] + (rotate(words[i- 2], 17) ^ rotate(words[i- 2], 19) ^ (words[i- 2] >> 10));
	}

	// seventh round
	x = h + f1(e,f,g) + 0x19a4c116 + words[48]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0x1e376c08 + words[49]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0x2748774c + words[50]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0x34b0bcb5 + words[51]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0x391c0cb3 + words[52]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0x4ed8aa4a + words[53]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0x5b9cca4f + words[54]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0x682e6ff3 + words[55]; y = f2(b,c,d); e += x; a = x + y;

	// extend to 64 words
	for (; i < 64; i++) {
		words[i] = words[i-16] + (rotate(words[i-15],  7) ^ rotate(words[i-15], 18) ^ (words[i-15] >>  3)) + words[i-7] + (rotate(words[i- 2], 17) ^ rotate(words[i- 2], 19) ^ (words[i- 2] >> 10));
	}

	// eigth round
	x = h + f1(e,f,g) + 0x748f82ee + words[56]; y = f2(a,b,c); d += x; h = x + y;
	x = g + f1(d,e,f) + 0x78a5636f + words[57]; y = f2(h,a,b); c += x; g = x + y;
	x = f + f1(c,d,e) + 0x84c87814 + words[58]; y = f2(g,h,a); b += x; f = x + y;
	x = e + f1(b,c,d) + 0x8cc70208 + words[59]; y = f2(f,g,h); a += x; e = x + y;
	x = d + f1(a,b,c) + 0x90befffa + words[60]; y = f2(e,f,g); h += x; d = x + y;
	x = c + f1(h,a,b) + 0xa4506ceb + words[61]; y = f2(d,e,f); g += x; c = x + y;
	x = b + f1(g,h,a) + 0xbef9a3f7 + words[62]; y = f2(c,d,e); f += x; b = x + y;
	x = a + f1(f,g,h) + 0xc67178f2 + words[63]; y = f2(b,c,d); e += x; a = x + y;

	// update hash
	m_hash[0] += a;
	m_hash[1] += b;
	m_hash[2] += c;
	m_hash[3] += d;
	m_hash[4] += e;
	m_hash[5] += f;
	m_hash[6] += g;
	m_hash[7] += h;
}

/// add arbitrary number of bytes
void SHA256::add(const void* data, uint64_t numBytes) {
	const uint8_t* current = (const uint8_t*) data;

	if (m_bufferSize > 0) {
		while (numBytes > 0 && m_bufferSize < BlockSize) {
			m_buffer[m_bufferSize++] = *current++;
			numBytes--;
		}
	}

	// full buffer
	if (m_bufferSize == BlockSize) {
		process_block(m_buffer);
		m_numBytes  += BlockSize;
		m_bufferSize = 0;
	}

	// no more data ?
	if (numBytes == 0) {
		return;
	}

	// process full blocks
	while (numBytes >= BlockSize) {
		process_block(current);
		current    += BlockSize;
		m_numBytes += BlockSize;
		numBytes   -= BlockSize;
	}

	// keep remaining bytes in buffer
	while (numBytes > 0) {
		m_buffer[m_bufferSize++] = *current++;
		numBytes--;
	}
}

void SHA256::add(const std::string& text) {
	add(text.c_str(), text.size());
}

/// process final block, less than 64 bytes
void SHA256::process_buffer() {
	// the input bytes are considered as bits strings, where the first bit is the most significant bit of the byte

	// - append "1" bit to message
	// - append "0" bits until message length in bit mod 512 is 448
	// - append length as 64 bit integer

	// number of bits
	uint64_t paddedLength = m_bufferSize * 8;

	// plus one bit set to 1 (always appended)
	paddedLength++;

	// number of bits must be (numBits % 512) = 448
	uint64_t lower11Bits = paddedLength & 511;
	if (lower11Bits <= 448) {
		paddedLength +=       448 - lower11Bits;
	}
	else {
		paddedLength += 512 + 448 - lower11Bits;
	}
	// convert from bits to bytes
	paddedLength /= 8;

	// only needed if additional data flows over into a second block
	uint8_t extra[BlockSize];

	// append a "1" bit, 128 => binary 10000000
	if (m_bufferSize < BlockSize) {
		m_buffer[m_bufferSize] = 128;
	}
	else {
		extra[0] = 128;
	}

	uint64_t i;
	for (i = m_bufferSize + 1; i < BlockSize; i++) {
		m_buffer[i] = 0;
	}
	for (; i < paddedLength; i++) {
		extra[i - BlockSize] = 0;
	}

	// add message length in bits as 64 bit number
	uint64_t msgBits = 8 * (m_numBytes + m_bufferSize);
	// find right position
	uint8_t* addLength;
	if (paddedLength < BlockSize) {
		addLength = m_buffer + paddedLength;
	}
	else {
		addLength = extra + paddedLength - BlockSize;
	}

	// must be big endian
	*addLength++ = (uint8_t)((msgBits >> 56) & 0xFF);
	*addLength++ = (uint8_t)((msgBits >> 48) & 0xFF);
	*addLength++ = (uint8_t)((msgBits >> 40) & 0xFF);
	*addLength++ = (uint8_t)((msgBits >> 32) & 0xFF);
	*addLength++ = (uint8_t)((msgBits >> 24) & 0xFF);
	*addLength++ = (uint8_t)((msgBits >> 16) & 0xFF);
	*addLength++ = (uint8_t)((msgBits >>  8) & 0xFF);
	*addLength   = (uint8_t)( msgBits        & 0xFF);

	// process blocks
	process_block(m_buffer);
	// flowed over into a second block ?
	if (paddedLength > BlockSize) {
		process_block(extra);
	}
}

/// return latest hash as 64 hex characters
std::string SHA256::get_hash() {
	// compute hash (as raw bytes)
	uint8_t* rawHash;
	get_raw_hash(&rawHash);

	// convert to hex string
	std::string result;
	result.reserve(2 * HashBytes);
	for (uint8_t i = 0; i < HashBytes; i++) {
		result += HEX_CHARS[(rawHash[i] >> 4) & 0xf];
		result += HEX_CHARS[ rawHash[i]       & 0xf];
	}

	delete[] rawHash;
	return result;
}

/// return latest hash as bytes
void SHA256::get_raw_hash(uint8_t** buffer) {
	// save old hash if buffer is partially filled
	uint32_t old_hash[HashValues];
	for (uint8_t i = 0; i < HashValues; i++) {
		old_hash[i] = m_hash[i];
	}

	// process remaining bytes
	process_buffer();

	uint8_t* out = new uint8_t[HashBytes];
	*buffer = out;  // return via arg
	
	for (uint8_t i = 0; i < HashValues; i++) {
		*out++ = (m_hash[i] >> 24) & 0xFF;
		*out++ = (m_hash[i] >> 16) & 0xFF;
		*out++ = (m_hash[i] >>  8) & 0xFF;
		*out++ =  m_hash[i]        & 0xFF;

		// restore old hash from "process_buffer();"
		m_hash[i] = old_hash[i];
	}
}

/// compute SHA256 of a memory block
std::string SHA256::operator()(const void* data, uint64_t numBytes) {
	reset();
	add(data, numBytes);
	return get_hash();
}

/// compute SHA256 of a string, excluding final zero
std::string SHA256::operator()(const std::string& text) {
	reset();
	add(text.c_str(), text.size());
	return get_hash();
}













/// same as reset()
SHA3::SHA3(SHA3_type bits) : m_blockSize(200 - 2 * (bits / 8)), m_bits(bits) { reset(); }


/// restart
void SHA3::reset() {
	for (size_t i = 0; i < StateSize; i++) {
		m_hash[i] = 0;
	}
	m_numBytes   = 0;
	m_bufferSize = 0;
}


/// constants and local helper functions
namespace {
	const unsigned int Rounds = 24;
	const uint64_t XorMasks[Rounds] = {
		0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
		0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
		0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
		0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
		0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
		0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
		0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
		0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
	};

	/// rotate left and wrap around to the right
	inline uint64_t rotateLeft(uint64_t x, uint8_t numBits) {
		return (x << numBits) | (x >> (64 - numBits));
	}

	/// convert litte vs big endian
	inline uint64_t swap(uint64_t x) {
	#if defined(__GNUC__) || defined(__clang__)
		return __builtin_bswap64(x);
	#endif
	#ifdef _MSC_VER
		return _byteswap_uint64(x);
	#endif
		return  (x >> 56) |
			((x >> 40) & 0x000000000000FF00ULL) |
			((x >> 24) & 0x0000000000FF0000ULL) |
			((x >>  8) & 0x00000000FF000000ULL) |
			((x <<  8) & 0x000000FF00000000ULL) |
			((x << 24) & 0x0000FF0000000000ULL) |
			((x << 40) & 0x00FF000000000000ULL) |
			(x << 56);
	}

	/// return x % 5 for 0 <= x <= 9
	unsigned int mod5(unsigned int x) {
		if (x < 5) {
			return x;
		}
		return x - 5;
	}
}

// process a full block
void SHA3::process_block(const void* data) {
#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
#define LITTLEENDIAN(x) swap(x)
#else
#define LITTLEENDIAN(x) (x)
#endif
	const uint64_t* data64 = (const uint64_t*) data;
	
	for (unsigned int i = 0; i < m_blockSize / 8; i++) {  // mix data into state
		m_hash[i] ^= LITTLEENDIAN(data64[i]);
	}
	
	for (unsigned int round = 0; round < Rounds; round++) {  // re-compute state
		// Theta
		uint64_t coefficients[5];
		for (unsigned int i = 0; i < 5; i++) {
			coefficients[i] = m_hash[i] ^ m_hash[i + 5] ^ m_hash[i + 10] ^ m_hash[i + 15] ^ m_hash[i + 20];
		}

		uint64_t temp;
		for (unsigned int i = 0; i < 5; i++) {
			uint64_t temp = coefficients[mod5(i + 4)] ^ rotateLeft(coefficients[mod5(i + 1)], 1);
			m_hash[i     ] ^= temp;
			m_hash[i +  5] ^= temp;
			m_hash[i + 10] ^= temp;
			m_hash[i + 15] ^= temp;
			m_hash[i + 20] ^= temp;
		}

		// Rho Pi
		uint64_t last =		m_hash[1];
		temp = m_hash[10];	m_hash[10] = rotateLeft(last,  1);	last = temp;
		temp = m_hash[ 7];	m_hash[ 7] = rotateLeft(last,  3);	last = temp;
		temp = m_hash[11];	m_hash[11] = rotateLeft(last,  6);	last = temp;
		temp = m_hash[17];	m_hash[17] = rotateLeft(last, 10);	last = temp;
		temp = m_hash[18];	m_hash[18] = rotateLeft(last, 15);	last = temp;
		temp = m_hash[ 3];	m_hash[ 3] = rotateLeft(last, 21);	last = temp;
		temp = m_hash[ 5];	m_hash[ 5] = rotateLeft(last, 28);	last = temp;
		temp = m_hash[16];	m_hash[16] = rotateLeft(last, 36);	last = temp;
		temp = m_hash[ 8];	m_hash[ 8] = rotateLeft(last, 45);	last = temp;
		temp = m_hash[21];	m_hash[21] = rotateLeft(last, 55);	last = temp;
		temp = m_hash[24];	m_hash[24] = rotateLeft(last,  2);	last = temp;
		temp = m_hash[ 4];	m_hash[ 4] = rotateLeft(last, 14);	last = temp;
		temp = m_hash[15];	m_hash[15] = rotateLeft(last, 27);	last = temp;
		temp = m_hash[23];	m_hash[23] = rotateLeft(last, 41);	last = temp;
		temp = m_hash[19];	m_hash[19] = rotateLeft(last, 56);	last = temp;
		temp = m_hash[13];	m_hash[13] = rotateLeft(last,  8);	last = temp;
		temp = m_hash[12];	m_hash[12] = rotateLeft(last, 25);	last = temp;
		temp = m_hash[ 2];	m_hash[ 2] = rotateLeft(last, 43);	last = temp;
		temp = m_hash[20];	m_hash[20] = rotateLeft(last, 62);	last = temp;
		temp = m_hash[14];	m_hash[14] = rotateLeft(last, 18);	last = temp;
		temp = m_hash[22];	m_hash[22] = rotateLeft(last, 39);	last = temp;
		temp = m_hash[ 9];	m_hash[ 9] = rotateLeft(last, 61);	last = temp;
		temp = m_hash[ 6];	m_hash[ 6] = rotateLeft(last, 20);	last = temp;
							m_hash[ 1] = rotateLeft(last, 44);

		// Chi
		for (unsigned int j = 0; j < StateSize; j += 5) {
			uint64_t temp_a = m_hash[j];
			uint64_t temp_b = m_hash[j + 1];

			m_hash[j]     ^= m_hash[j + 2]	& ~temp_b;
			m_hash[j + 1] ^= m_hash[j + 3]	& ~m_hash[j + 2];
			m_hash[j + 2] ^= m_hash[j + 4]	& ~m_hash[j + 3];
			m_hash[j + 3] ^= temp_a			& ~m_hash[j + 4];
			m_hash[j + 4] ^= temp_b			& ~temp_a;
		}

		// Iota
		m_hash[0] ^= XorMasks[round];
	}
}


/// add arbitrary number of bytes
void SHA3::add(const void* data, size_t numBytes) {
	const uint8_t* current = (const uint8_t*)data;
	if (m_bufferSize > 0) {  // copy data to buffer
		while (numBytes > 0 && m_bufferSize < m_blockSize) {
			m_buffer[m_bufferSize++] = *current++;
			numBytes--;
		}
	}
	
	if (m_bufferSize == m_blockSize) {  // full buffer
		process_block((void*)m_buffer);
		m_numBytes  += m_blockSize;
		m_bufferSize = 0;
	}
	
	if (numBytes == 0) {  // no more data ?
		return;
	}
	
	while (numBytes >= m_blockSize) {  // process full blocks
		process_block(current);
		current    += m_blockSize;
		m_numBytes += m_blockSize;
		numBytes   -= m_blockSize;
	}

	while (numBytes > 0) {  // keep remaining bytes in buffer
		m_buffer[m_bufferSize++] = *current++;
		numBytes--;
	}
}

void SHA3::add(const std::string& text) { add(text.c_str(), text.length()); }

// process everything left in the internal buffer
void SHA3::process_buffer() {
	size_t offset = m_bufferSize;  // add padding
	m_buffer[offset++] = 0x06;  // add a "1" byte
	while (offset < m_blockSize) {  // fill with zeros
		m_buffer[offset++] = 0;
	}

	m_buffer[offset - 1] |= 0x80;  // and add a single set bit
	process_block(m_buffer);
}


// return latest hash as 16 hex characters
std::string SHA3::get_hash() {
	unsigned char* hash;
	get_raw_hash(&hash);

	// ERROR
	unsigned int iter = m_bits / 8;
	std::string result;
	result.reserve(iter * 2);
	for (unsigned int i = 0; i < iter; i++) {
		result += HEX_CHARS[(hash[i] >> 4) & 0xf];
		result += HEX_CHARS[ hash[i]       & 0xf];
	}

	delete[] hash;
	return result;  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< error
}

void SHA3::get_raw_hash(unsigned char** buffer) {
	uint64_t old_hash[StateSize];

	memcpy(old_hash, m_hash, StateSize * 8);

	process_buffer();  // process remaining bytes
	
	unsigned int out_len = m_bits / 8;
	unsigned char* out = new unsigned char[out_len];

	memcpy(out, m_hash, out_len);
	memcpy(m_hash, old_hash, StateSize * 8);

	*buffer = out;  // return via arg
}

// compute SHA3 of a memory block
std::string SHA3::operator()(const void* data, size_t numBytes) {
	reset();
	add(data, numBytes);
	return get_hash();
}

// compute SHA3 of a string, excluding final zero
std::string SHA3::operator()(const std::string& text) {
	reset();
	add(text.c_str(), text.size());
	return get_hash();
}