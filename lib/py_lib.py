import os

from . import py_lib_wrap



# python function
clear_console = lambda: os.system("cls" if os.name in ["nt", "dos"] else "clear")

def clear_print(*args, **kwargs) -> None:
	clear_console();
	print(*args, **kwargs)


# c++ classes
class SHA256(py_lib_wrap.SHA256):
	def __init__(self) -> None:
		super(SHA256, self).__init__()


# c++ functions
def getpass(prompt: str, show_asterisk: bool = True) -> str:
	# encoding the prompt as a bytestring so that c++ can understand it
	# then decoding the received bytestring as ascii so that python can use it as a normal string
	return py_lib_wrap.getpass(prompt.encode(), show_asterisk).decode("ascii");