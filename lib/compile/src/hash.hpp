#pragma once

#include <string>
#ifndef _MSC_VER  // big endian architectures need #define __BYTE_ORDER __BIG_ENDIAN
#include <endian.h>
#endif


/// compute SHA256 hash
/** Usage:
    SHA256 sha256;
    std::string myHash  = sha256("Hello World");     // std::string
    std::string myHash2 = sha256("How are you", 11); // arbitrary data, 11 bytes

    // or in a streaming fashion:

    SHA256 sha256;
    while (more data available)
      sha256.add(pointer to fresh data, number of new bytes);
    std::string myHash3 = sha256.getHash();
  */
class Hash {
public:
	/// compute hash of a memory block
	virtual std::string operator()(const void* data, uint64_t numBytes) = 0;
	/// compute hash of a string, excluding final zero
	virtual std::string operator()(const std::string& text) = 0;

	/// add arbitrary number of bytes
	virtual void add(const void* data, uint64_t numBytes) = 0;
	virtual void add(const std::string& text) = 0;

	/// return latest hash as hex characters
	virtual std::string get_hash() = 0;

	/// restart
	virtual void reset() = 0;
};

/// SHA256 class derived from abstract class
class SHA256 : public Hash {
public:
	/// split into 64 byte blocks (=> 512 bits), hash is 32 bytes long
	enum { BlockSize = 64 /* 512 / 8 */, HashBytes = 32 };

	/// same as reset()
	SHA256();

	/// compute SHA256 of a memory block
	std::string operator()(const void* data, uint64_t numBytes) override;
	/// compute SHA256 of a string, excluding final zero
	std::string operator()(const std::string& text) override;

	/// add arbitrary number of bytes
	void add(const void* data, uint64_t numBytes) override;
	void add(const std::string& text) override;
	/// return latest hash as 64 hex characters
	std::string		get_hash() override;
	/// return latest hash as bytes
	void					get_raw_hash(unsigned char** buffer);

	/// restart
	void reset() override;

private:
	/// process 64 bytes
	void process_block(const void* data);
	/// process everything left in the internal buffer
	void process_buffer();

	/// size of processed data in bytes
	uint64_t m_numBytes;
	/// valid bytes in m_buffer
	uint64_t   m_bufferSize;
	/// bytes not processed yet
	uint8_t  m_buffer[BlockSize];

	enum { HashValues = HashBytes / 4 };
	/// hash, stored as integers
	uint32_t m_hash[HashValues];
};