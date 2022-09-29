#include "file.hpp"



uint64_t* CRC64_ECMA_LOOKUP_TABLE = nullptr;

// folder
uint64_t enc_file_components::folder::serialize(uint8_t** buffer) {
	uint64_t size = this->title_len + 3;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*ptr = this->title_len;
	memcpy(ptr + 1, this->title, this->title_len);
	ptr += this->title_len + 1;
	*((uint16_t*)ptr) = folder_id;
	return size;
}
void enc_file_components::folder::deserialize(uint8_t* buffer) {
	this->title_len = *buffer;
	buffer++;
	if (this->title) { delete[] this->title; }
	this->title = new char[this->title_len];
	memcpy(this->title, buffer, this->title_len);
	buffer += this->title_len;
	this->folder_id = *((uint16_t*)buffer);
}
enc_file_components::folder::~folder() {
	delete[] title;
}

// password
uint64_t enc_file_components::password::serialize(uint8_t** buffer) {
	uint64_t size = this->note_len + this->title_len + this->username_len + this->password_len + 7;
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
	return size;
}
void enc_file_components::password::deserialize(uint8_t* buffer) {
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
enc_file_components::password::~password() {
	delete[] title;
	delete[] username;
	delete[] password;
	delete[] note;
}

// note
uint64_t enc_file_components::note::serialize(uint8_t** buffer) {
	uint64_t size = this->note_len + this->title_len + 5;
	*buffer = new uint8_t[size];
	uint8_t* ptr = *buffer;

	*ptr = this->title_len;
	memcpy(ptr + 1, this->title, this->title_len);
	ptr += this->title_len + 1;
	*((uint16_t*)ptr) = this->note_len;
	memcpy(ptr + 2, this->note, this->note_len);
	ptr += this->note_len + 2;
	*((uint16_t*)ptr) = folder_id;
	return size;
}
void enc_file_components::note::deserialize(uint8_t* buffer) {
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
enc_file_components::note::~note() {
	delete[] title;
	delete[] note;
}

// personal_info
uint64_t enc_file_components::personal_info::serialize(uint8_t** buffer) {
	uint64_t size = this->note_len + this->title_len + this->first_name_len
		+ this->last_name_len + this->email_len + this->country_len
		+ this->province_len + this->city_len + this->street_len
		+ this->house_number_len + this->zip_code_len + 30;

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
	return size;
}
void enc_file_components::personal_info::deserialize(uint8_t* buffer) {
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
	if (this->phone) { delete[] this->phone; }
	this->phone = new char[16];
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
enc_file_components::personal_info::~personal_info() {
	delete[] title;
	delete[] first_name;
	delete[] last_name;
	delete[] email;
	delete[] phone;
	delete[] country;
	delete[] province;
	delete[] city;
	delete[] street;
	delete[] house_number;
	delete[] zip_code;
	delete[] note;
}

void enc_file_components::block::encrypt(void* data, const uint8_t* key) {
	if (this->cypher_text) { delete[] this->cypher_text; }
	AES a();

	uint8_t* raw_data = nullptr;
	uint64_t size = data.serialize(&raw_data);
	// TODO: PADDING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// TODO: PADDING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// TODO: PADDING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// TODO: PADDING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// TODO: PADDING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	this->AES_iv = generate_iv();
	// this->cypher_text = a.encrypt_CBC(raw_data, size, key, this->AES_iv);
}
void* enc_file_components::block::decrypt(const uint8_t* key) {
	switch (this->type) {
		case enc_file_components::block_data::types::folder:
			enc_file_components::folder			out();	break;
		case enc_file_components::block_data::types::password:
			enc_file_components::password		out();	break;
		case enc_file_components::block_data::types::note:
			enc_file_components::note			out();	break;
		case enc_file_components::block_data::types::personal_info:
			enc_file_components::personal_info	out();	break;
		default:
			return nullptr;
	};
	if (!this->cypher_text) { return nullptr; }
	uint8_t* buffer = new uint8_t[this->block_size];
	return nullptr;
}

uint64_t enc_file_components::block::serialize(uint8_t** buffer) {
	return 0;
}

void enc_file_components::block::deserialize(uint8_t* buffer) {

}


enc_file::enc_file(const std::string path, const std::string key) {
	if (!CRC64_ECMA_LOOKUP_TABLE) { CRC64_ECMA_LOOKUP_TABLE = init_crc64(crc_types::crc64_ecma); }

}
enc_file::~enc_file() {}