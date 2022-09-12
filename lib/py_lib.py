import os
import struct

from . import py_lib_wrap



# c++ constants
AESKeyLength_AES_128 = py_lib_wrap.AESKeyLength_AES_128
AESKeyLength_AES_192 = py_lib_wrap.AESKeyLength_AES_192
AESKeyLength_AES_256 = py_lib_wrap.AESKeyLength_AES_256



# c++ classes
class SHA256(py_lib_wrap.SHA256):
	def __init__(self) -> None:
		super(SHA256, self).__init__()

	@property
	def raw_hash(self) -> bytes:
		return self.get_raw_hash()


class AES(py_lib_wrap.AES):
	def __init__(self, key_lenght = AESKeyLength_AES_256) -> None:
		super(AES, self).__init__(key_lenght)



# c++ functions
def getpass(prompt: str, replacement: str = "*") -> str:
	# encoding the prompt as a bytestring so that c++ can understand it
	# then decoding the received bytestring as ascii so that python can use it as a normal string
	return py_lib_wrap.getpass(prompt, replacement.encode()[0]);



# python constants
class chs:  # (char style) class acts like a namespace
	END =		0
	NEGATIVE =	7
	RED =		91
	GREEN =		92
	WARNING =	93
	BLUE =		94
	PURPLE =	95
	CYAN =		96

	@staticmethod
	def get(code: int) -> str:
		return f"\033[{code}m"

	@classmethod
	def apply(cls, msg: str, code) -> str:
		return f"{cls.get(code)}{msg}{cls.get(cls.END)}"

# TODO: maybe move this to c++



# python function
clear_console = lambda: os.system("cls" if os.name in ["nt", "dos"] else "clear")

def clear_print(*args, **kwargs) -> None:
	clear_console();
	print(*args, **kwargs)