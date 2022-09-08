import os

from . import py_lib_wrap

# c++ constants
AESKeyLength_AES_128 = py_lib_wrap.AESKeyLength_AES_128
AESKeyLength_AES_192 = py_lib_wrap.AESKeyLength_AES_192
AESKeyLength_AES_256 = py_lib_wrap.AESKeyLength_AES_256

# c++ classes
class SHA256(py_lib_wrap.SHA256):
	def __init__(self) -> None:
		super(SHA256, self).__init__()

class AES(py_lib_wrap.AES):
	def __init__(self, key_lenght = AESKeyLength_AES_256) -> None:
		super(AES, self).__init__(key_lenght)


# c++ functions
def getpass(prompt: str, replacement: str = "*") -> str:
	# encoding the prompt as a bytestring so that c++ can understand it
	# then decoding the received bytestring as ascii so that python can use it as a normal string
	return py_lib_wrap.getpass(prompt, replacement.encode()[0]);


# python function
clear_console = lambda: os.system("cls" if os.name in ["nt", "dos"] else "clear")

def clear_print(*args, **kwargs) -> None:
	clear_console();
	print(*args, **kwargs)