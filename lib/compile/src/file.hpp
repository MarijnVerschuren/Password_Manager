#pragma once

#include "pybind11/pybind11.h"
namespace py = pybind11;

#include "math.hpp"
#include "encryption.hpp"
#include "hash.hpp"
#include "check.hpp"

#include <string>
#include <filesystem>
#include <fstream>



namespace efc {  // enc_file_components
	// data in these structs are stored from big to small
	// this is NOT how it will be stored in the file
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

		/* // store order:
			title_len
			title
			folder_id
		*/
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

		/* // store order:
			title_len
			title
			username_len
			username
			password_len
			password
			note_len
			note
			folder_id
		*/
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

		/* // store order:
			title_len
			title
			note_len
			note
			folder_id
		*/
	};
	class personal_info : public type {
	public:
		char[16] phone;  // 16 bytes
		char* title;
		char* first_name;
		char* last_name;
		char* email;
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

		/* // store order:
			title_len
			title
			first_name_len
			first_name
			last_name_len
			last_name
			email_len
			email
			phone_number	[16]
			country_len
			country
			province_len
			province
			city_len
			city
			street_len
			street
			house_number_len
			house_number
			zip_len
			zip
			note_len
			note
			folder_id
		*/
	};

	struct block {
	public:
		uint8_t AES_iv[16] =	{};					// 16 bytes
		uint64_t block_size =	0x0000000000000000;
		uint8_t* cypher_text =	nullptr;			// [block_size - 25] bytes
		uint64_t crc =			0x0000000000000000;	// crc64_ECMA
		uint8_t type =			0x00;

		void		encrypt(block_data data, const uint8_t* key);
		void*		decrypt(const uint8_t* key);

		uint64_t	serialize(uint8_t**);
		bool		deserialize(uint8_t*);	// return (crc) error

		/* // store order:
			block_size	= len(cypher_text) + 1 + 16 + 8
			block_type	[1]
			AES_iv		[16]
			cypher_text	[block_size - 25]
			crc			[8]
		*/
	};

	struct header {
		uint8_t salt[64];
		uint8_t hash[64];
	};
};


class enc_file {
public:
	enum error_types {
		file_handle_error = 0x01,
		incorrect_password = 0x02,
	};

	enc_file(const std::string path, const std::string key);
	~enc_file();

	uint8_t new_file();
	uint8_t open();
	void add_block();

private:
	std::string path;
	std::string key;
	bool is_open = false;
};