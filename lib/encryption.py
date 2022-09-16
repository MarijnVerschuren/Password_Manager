from .py_lib import AES as _AES


class AES(_AES):
	def __init__(self, key_type: _AES.AES_key = _AES.AES_256) -> None:
		super(AES, self).__init__(key_type)
		self.key_type = key_type

	def pad_plain_text(self, data: bytes) -> bytes:
		return data + (b"\x00" * (16 - (len(data) % 16)))

	def check_data_len(self, data: bytes) -> bool:
		return (len(data) % 16) == 0;

	def check_key_type(self, key: bytes) -> bool:
		if self.key_type == _AES.AES_256:	return len(key) == 32
		elif self.key_type == _AES.AES_192:	return len(key) == 24
		else:								return len(key) == 16  # AES_128

	def check_iv_len(self, iv: bytes) -> bool:
		return len(iv) == 16

	def encrypt_ECB(self, data: bytes or str, key: bytes) -> bytes:
		if type(data) == str: data = data.encode()
		if not self.check_key_type(key): return b"";  # fail
		return super(AES, self).encrypt_ECB(self.pad_plain_text(data), key)

	def decrypt_ECB(self, data: bytes or str, key: bytes) -> bytes:
		if type(data) == str: data = data.encode()
		if not (self.check_key_type(key) and self.check_data_len(data)): return b"";  # fail
		return super(AES, self).decrypt_ECB(data, key)

	def encrypt_CBC(self, data: bytes or str, key: bytes, iv: bytes) -> bytes:
		if type(data) == str: data = data.encode()
		if not (self.check_key_type(key) and self.check_iv_len(iv)): return b"";  # fail
		return super(AES, self).encrypt_CBC(self.pad_plain_text(data), key, iv)

	def decrypt_CBC(self, data: bytes or str, key: bytes, iv: bytes) -> bytes:
		if type(data) == str: data = data.encode()
		if not (self.check_key_type(key) and self.check_iv_len(iv) and self.check_data_len(data)): return b"";  # fail
		return super(AES, self).decrypt_CBC(data, key, iv)

	def encrypt_CFB(self, data: bytes or str, key: bytes, iv: bytes) -> bytes:
		if type(data) == str: data = data.encode()
		if not (self.check_key_type(key) and self.check_iv_len(iv)): return b"";  # fail
		return super(AES, self).encrypt_CFB(self.pad_plain_text(data), key, iv)

	def decrypt_CFB(self, data: bytes or str, key: bytes, iv: bytes) -> bytes:
		if type(data) == str: data = data.encode()
		if not (self.check_key_type(key) and self.check_iv_len(iv) and self.check_data_len(data)): return b"";  # fail
		return super(AES, self).decrypt_CFB(data, key, iv)



"""
# c++ constants
AESKeyLength_AES_128 = py_lib_wrap.AESKeyLength_AES_128
AESKeyLength_AES_192 = py_lib_wrap.AESKeyLength_AES_192
AESKeyLength_AES_256 = py_lib_wrap.AESKeyLength_AES_256

# c++ classes
class AES(py_lib_wrap.AES):
	def __init__(self, key_lenght = AESKeyLength_AES_256) -> None:
		super(AES, self).__init__(key_lenght)

	def CBC_encrypt(self, msg: bytes, key: bytes, iv: bytes) -> bytes:
		return self.EncryptCBC(msg, len(msg), key, iv)

"""