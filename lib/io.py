from .py_lib import getpass as _getpass
from .py_lib import print_hex_array



def getpass(prompt: str, replacement: str = "*") -> str:  # wrapper is used to enable default arguments
	return _getpass(prompt, replacement)