#pragma once

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "math.hpp"
#include "encryption.hpp"
#include "hash.hpp"
#include "check.hpp"

#include <string>



namespace enc_file_components {
	class type {
		enum types {
			null =			0x0,
			folder =		0x1,
			password =		0x2,
			note =			0x4,
			personal_info =	0x8,
		};

		virtual uint64_t	serialize(uint8_t** const uint8_t pad_to) =	NULL;
		virtual void		deserialize(uint8_t*) =						NULL;
	};

	class folder : public type {
	public:
		char* title;
		uint16_t folder_id;
		uint8_t title_len;

		uint64_t	serialize(uint8_t** const uint8_t pad_to);
		void		deserialize(uint8_t*);
		~folder();
	};
	class password : public type {
	public:
		char* title;
		char* username;
		char* password;
		char* note;
		uint16_t folder_id;
		uint16_t note_len;
		uint8_t title_len;
		uint8_t username_len;
		uint8_t password_len;

		uint64_t	serialize(uint8_t** const uint8_t pad_to);
		void		deserialize(uint8_t*);
		~password();
	};
	class note : public type {
	public:
		char* title;
		char* note;
		uint16_t folder_id;
		uint16_t note_len;
		uint8_t title_len;

		uint64_t	serialize(uint8_t** const uint8_t pad_to);
		void		deserialize(uint8_t*);
		~note();
	};
	class personal_info : public type {
	public:
		char* title;
		char* first_name;
		char* last_name;
		char* email;
		char* phone;	// 16 bytes
		char* country;
		char* province;
		char* city;
		char* street;
		char* house_number;
		char* zip_code;
		char* note;
		uint16_t folder_id;
		uint16_t note_len;
		uint8_t title_len;
		uint8_t first_name_len;
		uint8_t last_name_len;
		uint8_t email_len;
		uint8_t country_len;
		uint8_t province_len;
		uint8_t city_len;
		uint8_t street_len;
		uint8_t house_number_len;
		uint8_t zip_code_len;

		uint64_t	serialize(uint8_t**, const uint8_t pad_to);
		void		deserialize(uint8_t*);
		~personal_info();
	};

	struct block {
	public:
		uint64_t block_size =	0x0000000000000000;
		uint8_t* AES_iv =		nullptr;			// 16 bytes
		uint8_t* cypher_text =	nullptr;			// [block_size] bytes
		uint64_t crc =			0x0000000000000000;	// crc64_ECMA
		uint8_t type =			0x00;

		void		encrypt(block_data data, const uint8_t* key);
		void*		decrypt(const uint8_t* key);

		uint64_t	serialize(uint8_t**);
		bool		deserialize(uint8_t*);	// return (crc) error
	};
};


class enc_file {
public:
	enc_file(const std::string path, const std::string key);
	~enc_file();

	void add_block();

private:
	std::string path;
};