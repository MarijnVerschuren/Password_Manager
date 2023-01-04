#pragma once

#include "io.hpp"

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include <string>



class Hash {
public:
	virtual std::string operator()(const void* data, uint64_t numBytes) = 0;		// compute hash of a memory block
	virtual std::string operator()(const std::string& text) = 0;					// compute hash of a string, excluding final zero

	// add arbitrary number of bytes
	virtual void add(const void* data, uint64_t numBytes) = 0;
	virtual void add(const std::string& text) = 0;

	virtual std::string get_hash() = 0;												// return latest hash as hex characters
	virtual void get_raw_hash(unsigned char** buffer) = 0;							// return latest hash as an array

	virtual void reset() = 0;														// restart
};

// SHA256 class derived from abstract class
class SHA256 : public Hash {
public:
	enum { BlockSize = 64 /* 512 / 8 */, HashBytes = 32 };							// split into 64 byte blocks (=> 512 bits), hash is 32 bytes long

	SHA256();																		// same as reset()

	std::string operator()(const void* data, uint64_t numBytes) override;			// compute SHA256 of a memory block
	std::string operator()(const std::string& text) override;						// compute SHA256 of a string, excluding final zero

	// add arbitrary number of bytes
	void add(const void* data, uint64_t numBytes) override;
	void add(const std::string& text) override;

	std::string	get_hash() override;												// return latest hash as 64 hex characters
	void get_raw_hash(unsigned char** buffer) override;								// return latest hash as an array

	void reset() override;															// restart

private:
	void process_block(const void* data);											// process 64 bytes (a full block)
	void process_buffer();															// process everything left in the internal buffer

	uint64_t m_numBytes;															// size of processed data in bytes
	uint64_t m_bufferSize;															// valid bytes in m_buffer
	uint8_t m_buffer[BlockSize];													// bytes not processed yet

	enum { HashValues = HashBytes / 4 };
	uint32_t m_hash[HashValues];													// hash, stored as integers
};


// SHA3 class derived from abstract class
class SHA3 : public Hash {
public:
	enum SHA3_type { bits224 = 224, bits256 = 256, bits384 = 384, bits512 = 512 };	// algorithm variants

	explicit SHA3(SHA3_type bits = bits256);										// same as reset()

	std::string operator()(const void* data, size_t numBytes) override;				// compute hash of a memory block
	std::string operator()(const std::string& text) override;						// compute hash of a string, excluding final zero

	// add arbitrary number of bytes
	void add(const void* data, size_t numBytes) override;
	void add(const std::string& text) override;

	std::string get_hash() override;												// return latest hash as hex characters
	void get_raw_hash(unsigned char** buffer) override;

	void reset() override;															// restart
	
	SHA3_type	m_bits;																// variant
private:
	void process_block(const void* data);											// process a full block
	void process_buffer();															// process everything left in the internal buffer

	/// 1600 bits, stored as 25x64 bit, BlockSize is no more than 1152 bits (Keccak224)
	enum { StateSize = 25 /* 1600 / (8 * 8) */, MaxBlockSize = 144 /* 200 - 2 * (224 / 8) */ };

	uint64_t	m_hash[StateSize];													// hash
	uint64_t	m_numBytes;															// size of processed data in bytes
	size_t		m_blockSize;														// block size (less or equal to MaxBlockSize)
	size_t		m_bufferSize;														// valid bytes in m_buffer
	uint8_t		m_buffer[MaxBlockSize];												// bytes not processed yet
};