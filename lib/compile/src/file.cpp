#include "file.hpp"



uint64_t* CRC64_ECMA_LOOKUP_TABLE = nullptr;

// folder
uint64_t efc::folder::serialize(uint8_t** buffer, const uint8_t pad_to) {
	uint64_t size = this->title_len + 3;
	uint8_t padding = (pad_to - (size % pad_to));
	size += padding;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*ptr = this->title_len;
	memcpy(ptr + 1, this->title, this->title_len);
	ptr += this->title_len + 1;
	*((uint16_t*)ptr) = folder_id;
	ptr += 2;
	memset(ptr, 0x00, padding);

	return size;
}
void efc::folder::deserialize(uint8_t* buffer) {
	this->title_len = *buffer;
	buffer++;
	if (this->title) { delete[] this->title; }
	this->title = new char[this->title_len];
	memcpy(this->title, buffer, this->title_len);
	buffer += this->title_len;
	this->folder_id = *((uint16_t*)buffer);
}
efc::folder::~folder() {
	delete[] title;
}

// password
uint64_t efc::password::serialize(uint8_t** buffer, const uint8_t pad_to) {
	uint64_t size = this->note_len + this->title_len + this->username_len + this->password_len + 7;
	uint8_t padding = (pad_to - (size % pad_to));
	size += padding;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*ptr = this->title_len;
	memcpy(ptr + 1, this->title, this->title_len);
	ptr += this->title_len + 1;
	*ptr = this->username_len;
	memcpy(ptr + 1, this->username, this->username_len);
	ptr += this->username_len + 1;
	*ptr = this->password_len;
	memcpy(ptr + 1, this->password, this->password_len);
	ptr += this->password_len + 1;
	*((uint16_t*)ptr) = this->note_len;
	memcpy(ptr + 2, this->note, this->note_len);
	ptr += this->note_len + 2;
	*((uint16_t*)ptr) = folder_id;
	ptr += 2;
	memset(ptr, 0x00, padding);

	return size;
}
void efc::password::deserialize(uint8_t* buffer) {
	this->title_len = *buffer;
	buffer++;
	if (this->title) { delete[] this->title; }
	this->title = new char[this->title_len];
	memcpy(this->title, buffer, this->title_len);
	buffer += this->title_len;
	this->username_len = *buffer;
	buffer++;
	if (this->username) { delete[] this->username; }
	this->username = new char[this->username_len];
	memcpy(this->username, buffer, this->username_len);
	buffer += this->username_len;
	this->password_len = *buffer;
	buffer++;
	if (this->password) { delete[] this->password; }
	this->password = new char[this->password_len];
	memcpy(this->password, buffer, this->password_len);
	buffer += this->password_len;
	this->note_len = *((uint16_t*)buffer);
	buffer += 2;
	if (this->note) { delete[] this->note; }
	this->note = new char[this->note_len];
	memcpy(this->note, buffer, this->note_len);
	buffer += this->note_len;
	this->folder_id = *((uint16_t*)buffer);
}
efc::password::~password() {
	delete[] title;
	delete[] username;
	delete[] password;
	delete[] note;
}

// note
uint64_t efc::note::serialize(uint8_t** buffer, const uint8_t pad_to) {
	uint64_t size = this->note_len + this->title_len + 5;
	uint8_t padding = (pad_to - (size % pad_to));
	size += padding;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*ptr = this->title_len;
	memcpy(ptr + 1, this->title, this->title_len);
	ptr += this->title_len + 1;
	*((uint16_t*)ptr) = this->note_len;
	memcpy(ptr + 2, this->note, this->note_len);
	ptr += this->note_len + 2;
	*((uint16_t*)ptr) = folder_id;
	ptr += 2;
	memset(ptr, 0x00, padding);

	return size;
}
void efc::note::deserialize(uint8_t* buffer) {
	this->title_len = *buffer;
	buffer++;
	if (this->title) { delete[] this->title; }
	this->title = new char[this->title_len];
	memcpy(this->title, buffer, this->title_len);
	buffer += this->title_len;
	this->note_len = *((uint16_t*)buffer);
	buffer += 2;
	if (this->note) { delete[] this->note; }
	this->note = new char[this->note_len];
	memcpy(this->note, buffer, this->note_len);
	buffer += this->note_len;
	this->folder_id = *((uint16_t*)buffer);
}
efc::note::~note() {
	delete[] title;
	delete[] note;
}

// personal_info
uint64_t efc::personal_info::serialize(uint8_t** buffer, const uint8_t pad_to) {
	uint64_t size = this->note_len + this->title_len + this->first_name_len
		+ this->last_name_len + this->email_len + this->country_len
		+ this->province_len + this->city_len + this->street_len
		+ this->house_number_len + this->zip_code_len + 30;

	uint8_t padding = (pad_to - (size % pad_to));
	size += padding;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*ptr = this->title_len;
	memcpy(ptr + 1, this->title, this->title_len);
	ptr += this->title_len + 1;
	*ptr = this->first_name_len;
	memcpy(ptr + 1, this->first_name, this->first_name_len);
	ptr += this->first_name_len + 1;
	*ptr = this->last_name_len;
	memcpy(ptr + 1, this->last_name, this->last_name_len);
	ptr += this->last_name_len + 1;
	*ptr = this->email_len;
	memcpy(ptr + 1, this->email, this->email_len);
	ptr += this->email_len + 1;
	memcpy(ptr, this->phone, 16);
	ptr += 16;
	*ptr = this->country_len;
	memcpy(ptr + 1, this->country, this->country_len);
	ptr += this->country_len + 1;
	*ptr = this->province_len;
	memcpy(ptr + 1, this->province, this->province_len);
	ptr += this->province_len + 1;
	*ptr = this->city_len;
	memcpy(ptr + 1, this->city, this->city_len);
	ptr += this->city_len + 1;
	*ptr = this->street_len;
	memcpy(ptr + 1, this->street, this->street_len);
	ptr += this->street_len + 1;
	*ptr = this->house_number_len;
	memcpy(ptr + 1, this->house_number, this->house_number_len);
	ptr += this->house_number_len + 1;
	*ptr = this->zip_code_len;
	memcpy(ptr + 1, this->zip_code, this->zip_code_len);
	ptr += this->zip_code_len + 1;
	*((uint16_t*)ptr) = this->note_len;
	memcpy(ptr + 2, this->note, this->note_len);
	ptr += this->note_len + 2;
	*((uint16_t*)ptr) = folder_id;
	ptr += 2;
	memset(ptr, 0x00, padding);

	return size;
}
void efc::personal_info::deserialize(uint8_t* buffer) {
	this->title_len = *buffer;
	buffer++;
	if (this->title) { delete[] this->title; }
	this->title = new char[this->title_len];
	memcpy(this->title, buffer, this->title_len);
	buffer += this->title_len;
	this->first_name_len = *buffer;
	buffer++;
	if (this->first_name) { delete[] this->first_name; }
	this->first_name = new char[this->first_name_len];
	memcpy(this->first_name, buffer, this->first_name_len);
	buffer += this->first_name_len;
	this->last_name_len = *buffer;
	buffer++;
	if (this->last_name) { delete[] this->last_name; }
	this->last_name = new char[this->last_name_len];
	memcpy(this->last_name, buffer, this->last_name_len);
	buffer += this->last_name_len;
	this->email_len = *buffer;
	buffer++;
	if (this->email) { delete[] this->email; }
	this->email = new char[this->email_len];
	memcpy(this->email, buffer, this->email_len);
	buffer += this->email_len;
	memcpy(this->phone, buffer, 16);
	buffer += 16;
	this->country_len = *buffer;
	buffer++;
	if (this->country) { delete[] this->country; }
	this->country = new char[this->country_len];
	memcpy(this->country, buffer, this->country_len);
	buffer += this->country_len;
	this->province_len = *buffer;
	buffer++;
	if (this->province) { delete[] this->province; }
	this->province = new char[this->province_len];
	memcpy(this->province, buffer, this->province_len);
	buffer += this->province_len;
	this->city_len = *buffer;
	buffer++;
	if (this->city) { delete[] this->city; }
	this->city = new char[this->city_len];
	memcpy(this->city, buffer, this->city_len);
	buffer += this->city_len;
	this->street_len = *buffer;
	buffer++;
	if (this->street) { delete[] this->street; }
	this->street = new char[this->street_len];
	memcpy(this->street, buffer, this->street_len);
	buffer += this->street_len;
	this->house_number_len = *buffer;
	buffer++;
	if (this->house_number) { delete[] this->house_number; }
	this->house_number = new char[this->house_number_len];
	memcpy(this->house_number, buffer, this->house_number_len);
	buffer += this->house_number_len;
	this->zip_code_len = *buffer;
	buffer++;
	if (this->zip_code) { delete[] this->zip_code; }
	this->zip_code = new char[this->zip_code_len];
	memcpy(this->zip_code, buffer, this->zip_code_len);
	buffer += this->zip_code_len;
	this->note_len = *((uint16_t*)buffer);
	buffer += 2;
	if (this->note) { delete[] this->note; }
	this->note = new char[this->note_len];
	memcpy(this->note, buffer, this->note_len);
	buffer += this->note_len;
	this->folder_id = *((uint16_t*)buffer);
}
efc::personal_info::~personal_info() {
	delete[] title;
	delete[] first_name;
	delete[] last_name;
	delete[] email;
	delete[] country;
	delete[] province;
	delete[] city;
	delete[] street;
	delete[] house_number;
	delete[] zip_code;
	delete[] note;
}


uint64_t efc::raw_block::serialize(uint8_t** buffer, uint8_t pad_to) {
	switch (this->type) {
	case efc::type::folder:
		return ((efc::folder*)data)->serialize(buffer, pad_to);
	case efc::type::password:
		return ((efc::password*)data)->serialize(buffer, pad_to);
	case efc::type::note:
		return ((efc::note*)data)->serialize(buffer, pad_to);
	case efc::type::personal_info:
		return ((efc::personal_info*)data)->serialize(buffer, pad_to);
	default: return 0;  // fail
	}
}

void efc::enc_block::deserialize(uint8_t* buffer) {
	switch (this->type) {
		case efc::type::folder: {
			efc::folder* folder_data = new efc::folder;
			folder_data->deserialize(buffer);
			out->data = folder_data;
			return;
		}
		case efc::type::password: {
			efc::password* password_data = new efc::password;
			password_data->deserialize(buffer);
			out->data = password_data;
			return;
		}
		case efc::type::note: {
			efc::note* note_data = new efc::note;
			note_data->deserialize(buffer);
			out->data = note_data;
			return;
		}
		case efc::type::personal_info: {
			efc::personal_info*	personal_info_data = new efc::personal_info;
			personal_info_data->deserialize(buffer);
			out->data = personal_info_data;
			return;
		}
		default: { return; }  // fail (leave data as nullptr)
	};
}

void efc::enc_block::encrypt(efc::raw_block* data, const uint8_t* key) {
	if (this->cypher_text) { delete[] this->cypher_text; }
	this->type = data->type;
	AES a;
	uint8_t* raw_data = nullptr;
	this->block_size = data->serialize(&raw_data, (uint8_t)a.blockBytesLen);
	memcpy(this->AES_iv, generate_iv(), 16);
	this->cypher_text = a.encrypt_CBC(raw_data, (uint32_t)this->block_size, key, this->AES_iv);
	delete[] raw_data;
}
efc::raw_block* efc::enc_block::decrypt(const uint8_t* key) {
	if (!this->cypher_text) { return nullptr; }
	AES a;
	uint8_t* buffer = a.decrypt_CBC(this->cypher_text, (uint32_t)this->block_size, key, this->AES_iv);
	raw_block* out = new raw_block;
	out->type = this->type;
	out->deserialize(buffer);
	return out;
}

uint64_t efc::enc_block::serialize(uint8_t** buffer) {
	uint64_t size = this->block_size + 8;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*((uint64_t*)ptr) = this->block_size;
	ptr += 8;
	memcpy(ptr, this->AES_iv, 16);
	ptr += 16;
	*ptr = this->type;
	memcpy(ptr + 1, this->cypher_text, this->block_size);
	ptr += this->block_size + 1;
	*((uint64_t*)ptr) = crc_64(*buffer, size - 8, CRC64_ECMA_LOOKUP_TABLE);

	return size;
}

bool efc::enc_block::deserialize(uint8_t* buffer, uint64_t block_size) {
	this->block_size = block_size;
	uint8_t* ptr = buffer;
	memcpy(this->AES_iv, ptr, 16);
	ptr += 16;
	this->type = *ptr;
	if (this->cypher_text) { delete[] this->cypher_text; }
	memcpy(this->cypher_text, ptr + 1, this->block_size);
	ptr += this->block_size + 1;
	uint64_t crc = *((uint64_t*)ptr);
	
	return crc != crc_64(buffer, this->block_size + 25, CRC64_ECMA_LOOKUP_TABLE);
}



uint8_t enc_file::new_file(const std::string path, const std::string key) {
	// reset internal state of the class if the path is not the same
	if (this->path != path) { reset(); this->path = path; }
	this->key = key;
	// init the lokup table for the crc algorithem used
	if (!CRC64_ECMA_LOOKUP_TABLE) { CRC64_ECMA_LOOKUP_TABLE = init_crc64(crc_types::crc64_ecma); }
	// open the file if the path already exists 
	if (std::filesystem::exists(this->path)) { return open(this->path, this->key); }
	// write file
	std::ofstream file(this->path, std::ios::binary | std::ios::trunc | std::ios::out);
	if (!file || !file.is_open()) { return file_handle_error; }
	efc::header file_header;
	memcpy(file_header.salt, generate_salt(64), 64);
	SHA3 s(SHA3::bits512);
	s.add(this->key); s.add(file_header.salt, 64);
	uint8_t* current_hash; s.get_raw_hash(&current_hash);
	memcpy(file_header.hash, current_hash, 64);
	file.write((char*)&file_header, 128);
	file.close();
	this->authorized = true;
	return 0;  // ok
}

uint8_t enc_file::open(const std::string path, const std::string key) {
	// reset internal state of the class if the path is not the same
	if (this->path != path) { reset(); this->path = path; }
	this->key = key;
	// init the lokup table for the crc algorithem used
	if (!CRC64_ECMA_LOOKUP_TABLE) { CRC64_ECMA_LOOKUP_TABLE = init_crc64(crc_types::crc64_ecma); }
	// create the file if the path doesnt exists 
	if (!std::filesystem::exists(this->path)) { return new_file(this->path, this->key); }
	// check if the file has a header and read it
	efc::header file_header;
	std::ifstream file(this->path, std::ios::binary | std::ios::in | std::ios::ate);
	if (!file || !file.is_open()) { return file_handle_error; }
	uint64_t file_size = file.tellg();
	if (file_size < 128) { return new_file(this->path, this->key); }  // the file exists but has no data in it
	file.seekg(0);	// go to the start of the file
	file.read((char*)&file_header, 128);  // populate header struct
	SHA3 s(SHA3::bits512);
	s.add(this->key); s.add(file_header.salt, 64);
	uint8_t* current_hash; s.get_raw_hash(&current_hash);
	std::string expect((char*)file_header.hash, 64);
	std::string current((char*)current_hash, 64);
	file.close();
	if (expect != current) { return incorrect_password; }	// error
	this->authorized = true;
	return load();
}

void enc_file::add_block() {

}

uint8_t enc_file::load() {
	std::ifstream file(this->path, std::ios::binary | std::ios::in | std::ios::ate);
	if (!file || !file.is_open()) { return file_handle_error; }
	uint64_t bytes_left = file.tellg();
	file.seekg(128);	// skip header
	bytes_left -= 128;

	uint64_t block_size;
	uint8_t* buffer = nullptr;
	efc::enc_block block;

	while (bytes_left > 8) {
		file.read(&block_size, 8); bytes_left -= 8;
		if (block_size > bytes_left) { return unexpected_eof; }
		uint8_t* buffer = new uint8_t[block_size];
		file.read(buffer, block_size); bytes_left -= block_size;
		block.deserialize(buffer, block_size);
		this->enc_blocks.push_back(block);  // hope for copy <<<<<<<<<<<<<<<<<<<<<<<<<<<
		delete[] buffer;
	}
	return decrypt();
}

uint8_t enc_file::store() {
	for (uint64_t i = 0; i < this->enc_blocks.length(); i++) {
		// todo
	}
	return 0;
}

uint8_t enc_file::decrypt() {
	// todo
	return 0;
}

uint8_t enc_file::encrypt() {
	// todo
	return 0;
}

void enc_file::reset() {
	this->enc_blocks.empty();
	this->raw_blocks.empty();
	this->authorized = false;
	this->path = "";
	this->key = "";
}