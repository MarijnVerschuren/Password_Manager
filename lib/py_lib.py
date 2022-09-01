import os

from . import py_lib_wrap



clear_console = lambda: os.system("cls" if os.name in ["nt", "dos"] else "clear")

def getpass(prompt: str, show_asterisk: bool = True) -> str:
	# encoding the prompt as a bytestring so that c++ can understand it
	# then decoding the received bytestring as ascii so that python can use it as a normal string
	return py_lib_wrap.getpass(prompt.encode(), show_asterisk).decode("ascii");