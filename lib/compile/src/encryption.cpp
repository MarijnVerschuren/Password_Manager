#include "encryption.hpp"



uint8_t* generate_iv() {
	std::random_device rand;
	uint8_t* out = new uint8_t[16];
	((uint32_t*)out)[0] = rand();
	((uint32_t*)out)[1] = rand();
	((uint32_t*)out)[2] = rand();
	((uint32_t*)out)[3] = rand();
	return out;
}


AES::AES(const AES_key_type keyLength) {
	this->Nb = 4;
	switch (keyLength) {
		case AES_key_type::AES_128:
		this->Nk = 4;
		this->Nr = 10;
		break;
		case AES_key_type::AES_192:
		this->Nk = 6;
		this->Nr = 12;
		break;
		case AES_key_type::AES_256:
		this->Nk = 8;
		this->Nr = 14;
		break;
	}

	blockBytesLen = 4 * this->Nb * sizeof(unsigned char);
}

unsigned char *AES::encrypt_ECB(const unsigned char* in, unsigned int inLen, const unsigned char* key) {
	check_length(inLen);
	unsigned char *out = new unsigned char[inLen];
	unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
	key_expansion(key, roundKeys);
	for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
		encrypt_block(in + i, out + i, roundKeys);
	}

	delete[] roundKeys;

	return out;
}

unsigned char *AES::decrypt_ECB(const unsigned char* in, unsigned int inLen, const unsigned char* key) {
	check_length(inLen);
	unsigned char *out = new unsigned char[inLen];
	unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
	key_expansion(key, roundKeys);
	for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
		decrypt_block(in + i, out + i, roundKeys);
	}

	delete[] roundKeys;

	return out;
}

unsigned char *AES::encrypt_CBC(const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char *iv) {
	check_length(inLen);
	unsigned char *out = new unsigned char[inLen];
	unsigned char *block = new unsigned char[blockBytesLen];
	unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
	key_expansion(key, roundKeys);
	memcpy(block, iv, blockBytesLen);
	for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
		xor_blocks(block, in + i, block, blockBytesLen);
		encrypt_block(block, out + i, roundKeys);
		memcpy(block, out + i, blockBytesLen);
	}

	delete[] block;
	delete[] roundKeys;

	return out;
}

unsigned char *AES::decrypt_CBC(const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char *iv) {
	check_length(inLen);
	unsigned char *out = new unsigned char[inLen];
	unsigned char *block = new unsigned char[blockBytesLen];
	unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
	key_expansion(key, roundKeys);
	memcpy(block, iv, blockBytesLen);
	for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
		decrypt_block(in + i, out + i, roundKeys);
		xor_blocks(block, out + i, out + i, blockBytesLen);
		memcpy(block, in + i, blockBytesLen);
	}

	delete[] block;
	delete[] roundKeys;

	return out;
}

unsigned char *AES::encrypt_CFB(const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char *iv) {
	check_length(inLen);
	unsigned char *out = new unsigned char[inLen];
	unsigned char *block = new unsigned char[blockBytesLen];
	unsigned char *encryptedBlock = new unsigned char[blockBytesLen];
	unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
	key_expansion(key, roundKeys);
	memcpy(block, iv, blockBytesLen);
	for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
		encrypt_block(block, encryptedBlock, roundKeys);
		xor_blocks(in + i, encryptedBlock, out + i, blockBytesLen);
		memcpy(block, out + i, blockBytesLen);
	}

	delete[] block;
	delete[] encryptedBlock;
	delete[] roundKeys;

	return out;
}

unsigned char *AES::decrypt_CFB(const unsigned char* in, unsigned int inLen, const unsigned char* key, const unsigned char *iv) {
	check_length(inLen);
	unsigned char *out = new unsigned char[inLen];
	unsigned char *block = new unsigned char[blockBytesLen];
	unsigned char *encryptedBlock = new unsigned char[blockBytesLen];
	unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
	key_expansion(key, roundKeys);
	memcpy(block, iv, blockBytesLen);
	for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
		encrypt_block(block, encryptedBlock, roundKeys);
		xor_blocks(in + i, encryptedBlock, out + i, blockBytesLen);
		memcpy(block, in + i, blockBytesLen);
	}

	delete[] block;
	delete[] encryptedBlock;
	delete[] roundKeys;

	return out;
}

void AES::check_length(unsigned int len) {
	if (len % blockBytesLen != 0) {
		throw std::length_error("Plaintext length must be divisible by " + std::to_string(blockBytesLen));
	}
}

void AES::encrypt_block(const unsigned char* in, unsigned char* out, unsigned char* roundKeys) {
	unsigned char **state = new unsigned char *[4];
	state[0] = new unsigned char[4 * Nb];
	int i, j, round;
	for (i = 0; i < 4; i++) {
		state[i] = state[0] + Nb * i;
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
			state[i][j] = in[i + 4 * j];
		}
	}

	add_round_key(state, roundKeys);

	for (round = 1; round <= Nr - 1; round++) {
		sub_bytes(state);
		shift_rows(state);
		mix_columns(state);
		add_round_key(state, roundKeys + round * 4 * Nb);
	}

	sub_bytes(state);
	shift_rows(state);
	add_round_key(state, roundKeys + Nr * 4 * Nb);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
		out[i + 4 * j] = state[i][j];
		}
	}

	delete[] state[0];
	delete[] state;
}

void AES::decrypt_block(const unsigned char* in, unsigned char* out, unsigned char* roundKeys) {
	unsigned char **state = new unsigned char *[4];
	state[0] = new unsigned char[4 * Nb];
	int i, j, round;
	for (i = 0; i < 4; i++) {
		state[i] = state[0] + Nb * i;
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
			state[i][j] = in[i + 4 * j];
		}
	}

	add_round_key(state, roundKeys + Nr * 4 * Nb);

	for (round = Nr - 1; round >= 1; round--) {
		inv_sub_bytes(state);
		inv_shift_rows(state);
		add_round_key(state, roundKeys + round * 4 * Nb);
		inv_mix_columns(state);
	}

	inv_sub_bytes(state);
	inv_shift_rows(state);
	add_round_key(state, roundKeys);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
			out[i + 4 * j] = state[i][j];
		}
	}

	delete[] state[0];
	delete[] state;
}

void AES::sub_bytes(unsigned char **state) {
	int i, j;
	unsigned char t;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
			t = state[i][j];
			state[i][j] = sbox[t / 16][t % 16];
		}
	}
}

void AES::shift_row(unsigned char **state, int i, int n) { // shift row i on n positions
	unsigned char *tmp = new unsigned char[Nb];
	for (int j = 0; j < Nb; j++) {
		tmp[j] = state[i][(j + n) % Nb];
	}
	memcpy(state[i], tmp, Nb * sizeof(unsigned char));

	delete[] tmp;
}

void AES::shift_rows(unsigned char **state) {
	shift_row(state, 1, 1);
	shift_row(state, 2, 2);
	shift_row(state, 3, 3);
}

unsigned char AES::x_time(unsigned char b)  // multiply on x
{
	return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
}

void AES::mix_columns(unsigned char **state) {
	unsigned char temp_state[4][4];

	for (size_t i = 0; i < 4; ++i) {
		memset(temp_state[i], 0, 4);
	}

	for (size_t i = 0; i < 4; ++i) {
		for (size_t k = 0; k < 4; ++k) {
			for (size_t j = 0; j < 4; ++j) {
				if (CMDS[i][k] == 1)
					temp_state[i][j] ^= state[k][j];
				else
					temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
			}
		}
	}

	for (size_t i = 0; i < 4; ++i) {
		memcpy(state[i], temp_state[i], 4);
	}
}

void AES::add_round_key(unsigned char **state, unsigned char *key) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
		state[i][j] = state[i][j] ^ key[i + 4 * j];
		}
	}
}

void AES::sub_word(unsigned char *a) {
	int i;
	for (i = 0; i < 4; i++) {
		a[i] = sbox[a[i] / 16][a[i] % 16];
	}
}

void AES::rot_word(unsigned char *a) {
	unsigned char c = a[0];
	a[0] = a[1];
	a[1] = a[2];
	a[2] = a[3];
	a[3] = c;
}

void AES::xor_words(unsigned char *a, unsigned char *b, unsigned char *c) {
	int i;
	for (i = 0; i < 4; i++) {
		c[i] = a[i] ^ b[i];
	}
}

void AES::rcon(unsigned char *a, int n) {
	int i;
	unsigned char c = 1;
	for (i = 0; i < n - 1; i++) {
		c = x_time(c);
	}

	a[0] = c;
	a[1] = a[2] = a[3] = 0;
}

void AES::key_expansion(const unsigned char* key, unsigned char* w) {
	unsigned char *temp = new unsigned char[4];
	unsigned char *data = new unsigned char[4];

	int i = 0;
	while (i < 4 * Nk) {
		w[i] = key[i];
		i++;
	}

	i = 4 * Nk;
	while (i < 4 * Nb * (Nr + 1)) {
		temp[0] = w[i - 4 + 0];
		temp[1] = w[i - 4 + 1];
		temp[2] = w[i - 4 + 2];
		temp[3] = w[i - 4 + 3];

		if (i / 4 % Nk == 0) {
			rot_word(temp);
			sub_word(temp);
			rcon(data, i / (Nk * 4));
			xor_words(temp, data, temp);
		} else if (Nk > 6 && i / 4 % Nk == 4) {
			sub_word(temp);
		}

		w[i + 0] = w[i - 4 * Nk] ^ temp[0];
		w[i + 1] = w[i + 1 - 4 * Nk] ^ temp[1];
		w[i + 2] = w[i + 2 - 4 * Nk] ^ temp[2];
		w[i + 3] = w[i + 3 - 4 * Nk] ^ temp[3];
		i += 4;
	}

	delete[] data;
	delete[] temp;
}

void AES::inv_sub_bytes(unsigned char **state) {
	int i, j;
	unsigned char t;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < Nb; j++) {
		t = state[i][j];
		state[i][j] = inv_sbox[t / 16][t % 16];
		}
	}
}

void AES::inv_mix_columns(unsigned char **state) {
	unsigned char temp_state[4][4];

	for (size_t i = 0; i < 4; ++i) {
		memset(temp_state[i], 0, 4);
	}

	for (size_t i = 0; i < 4; ++i) {
		for (size_t k = 0; k < 4; ++k) {
			for (size_t j = 0; j < 4; ++j) {
				temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
			}
		}
	}

	for (size_t i = 0; i < 4; ++i) {
		memcpy(state[i], temp_state[i], 4);
	}
}

void AES::inv_shift_rows(unsigned char **state) {
	shift_row(state, 1, Nb - 1);
	shift_row(state, 2, Nb - 2);
	shift_row(state, 3, Nb - 3);
}

void AES::xor_blocks(const unsigned char *a, const unsigned char *b, unsigned char *c, unsigned int len) {
	for (unsigned int i = 0; i < len; i++) {
		c[i] = a[i] ^ b[i];
	}
}

std::vector<unsigned char> AES::array_to_vector(unsigned char *a, unsigned int len) {
	std::vector<unsigned char> v(a, a + len * sizeof(unsigned char));
	return v;
}

unsigned char *AES::vector_to_array(std::vector<unsigned char> &a) {
	return a.data();
}

std::vector<unsigned char> AES::encrypt_ECB(std::vector<unsigned char> in, std::vector<unsigned char> key) {
	unsigned char *out = encrypt_ECB(vector_to_array(in), (unsigned int)in.size(), vector_to_array(key));
	std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
	delete[] out;
	return v;
}

std::vector<unsigned char> AES::decrypt_ECB(std::vector<unsigned char> in, std::vector<unsigned char> key) {
	unsigned char *out = decrypt_ECB(vector_to_array(in), (unsigned int)in.size(), vector_to_array(key));
	std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
	delete[] out;
	return v;
}

std::vector<unsigned char> AES::encrypt_CBC(std::vector<unsigned char> in, std::vector<unsigned char> key, std::vector<unsigned char> iv) {
	unsigned char *out = encrypt_CBC(vector_to_array(in), (unsigned int)in.size(), vector_to_array(key), vector_to_array(iv));
	std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
	delete[] out;
	return v;
}

std::vector<unsigned char> AES::decrypt_CBC(std::vector<unsigned char> in, std::vector<unsigned char> key, std::vector<unsigned char> iv) {
	unsigned char *out = decrypt_CBC(vector_to_array(in), (unsigned int)in.size(), vector_to_array(key), vector_to_array(iv));
	std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
	delete[] out;
	return v;
}

std::vector<unsigned char> AES::encrypt_CFB(std::vector<unsigned char> in, std::vector<unsigned char> key, std::vector<unsigned char> iv) {
	unsigned char *out = encrypt_CFB(vector_to_array(in), (unsigned int)in.size(), vector_to_array(key), vector_to_array(iv));
	std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
	delete[] out;
	return v;
}

std::vector<unsigned char> AES::decrypt_CFB(std::vector<unsigned char> in, std::vector<unsigned char> key, std::vector<unsigned char> iv) {
	unsigned char *out = decrypt_CFB(vector_to_array(in), (unsigned int)in.size(), vector_to_array(key), vector_to_array(iv));
	std::vector<unsigned char> v = array_to_vector(out, (unsigned int)in.size());
	delete[] out;
	return v;
}