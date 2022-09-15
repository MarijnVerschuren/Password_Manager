# from .py_lib import




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