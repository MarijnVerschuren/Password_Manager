from .py_lib import getpass as _getpass

# c++ functions
def getpass(prompt: str, replacement: str = "*", release_gil: bool = False) -> str:  # wrapper is used to enable default arguments
	return _getpass(prompt, replacement, release_gil)