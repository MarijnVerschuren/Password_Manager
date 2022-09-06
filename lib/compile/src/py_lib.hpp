#pragma once


#include <iostream>
#include <string>
#include <windows.h>
#include <stdint.h>

#define CON_BACKSPACE 8
#define CON_RETURN 13  // Carriage Return



/// abstract base class
class Hash {
public:
  /// compute hash of a memory block
  virtual std::string operator()(const void* data, uint64_t numBytes) = 0;
  /// compute hash of a string, excluding final zero
  virtual std::string operator()(const std::string& text) = 0;

  /// add arbitrary number of bytes
  virtual void add(const void* data, uint64_t numBytes) = 0;

  /// return latest hash as hex characters
  virtual std::string getHash() = 0;

  /// restart
  virtual void reset() = 0;
};



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

  /// return latest hash as 64 hex characters
  std::string getHash() override;
  /// return latest hash as bytes
  void        getHash(unsigned char buffer[HashBytes]);

  /// restart
  void reset() override;

private:
  /// process 64 bytes
  void processBlock(const void* data);
  /// process everything left in the internal buffer
  void processBuffer();

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


std::string getpass(const char* prompt, bool show_asterisk = true);
std::string sha256_test();