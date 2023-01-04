"""
		includes
"""
import sys, os		# py libraries
from lib import *	# c++ library



"""
		globals
"""
root_folder = os.path.dirname(os.path.abspath(__file__))
data_folder = os.path.join(root_folder, "data")

__all__ = [
	"root_folder",
	"data_folder",

	"Lockbox",
	"set_logger"
]



"""
		exceptions
"""
class invalid_lockbox_name(Exception):
	"""
	Exception thrown when lockbox name is invalid cases include:
		- name taken					(create)
		- name not found				(login)
		- invalid characters
		- name too short or long
	"""
class invalid_password(Exception):
	"""
	Exception thrown when the password is invalid cases include:
		- password incorrect			(login)
		- passwords do not match		(create)
		- password too short or long
	"""



"""
		decorators and other helpers
"""
class Logger:
	def __init__(self, func: callable) -> None:
		self.logger_func = func

	def catch(self: object, func: callable) -> callable:
		def wrapper(*args, **kwargs) -> any:
			try:					return func(*args, **kwargs)
			except Exception as e:	self.logger_func(f"Exception occurred: {e}")
			return None
		wrapper.__name__ = func.__name__  # to prevent overwriting existing endpoints for "wrapper"
		return wrapper

def print_error(msg: str) -> None:
	print(chs.apply(msg, chs.RED))



"""
		functionality
"""
class Lockbox:
	logger = Logger(print_error)  # default logger (print in red)

	def __init__(
		self: object,
	) -> None:
		self.sha3_512 = SHA3(SHA3.bits512)
		self.name = None
		self.path = None
		self.key = None

	@logger.catch
	def new(self: object, name: str, key_0: str, key_1: str) -> None:
		self.name = name
		if key_0 != key_1:							raise invalid_password("passwords do not match")
		self.key = key_0;
		self.path =	os.path.join(data_folder, self.name)
		if os.path.exists(self.path):
			if os.path.getsize(self.path) >= 128:	raise invalid_lockbox_name("name taken")
			# if the file size is smaller than 128B the file is not taken as it has no header
		salt = os.urandom(64)
		self.sha3_512.add(self.key.encode("ascii") + salt)
		with open(self.path, "wb") as ofile:		# overwrite
			ofile.write(salt)						# 64B
			ofile.write(self.sha3_512.raw_hash)		# 64B
			ofile.close()
		self.sha3_512.reset()

	@logger.catch
	def unlock(self: object, name: str, key: str) -> None:
		self.name =	name;
		self.key = key
		self.path =	os.path.join(data_folder, self.name)
		if not os.path.exists(self.path):			raise invalid_lockbox_name("name not found")
		with open(self.path, "rb") as ifile:
			salt = ifile.read(64)
			_hash = ifile.read(64)
			ifile.close()
		self.sha3_512.add(key.encode("ascii") + salt)
		if self.sha3_512.raw_hash != _hash:			raise invalid_password("password incorrect")
		self.sha3_512.reset()
		return self.read_blocks()

	@logger.catch
	def read_blocks(self: object) -> str or None:
		pass


# TODO: rewrite above class to functions and holder classes (in app class for logger instance)


def set_logger(logger_func: callable) -> None:
	Lockbox.logger.logger_func = logger_func



# https://github.com/hoffstadt/DearPyGui
# https://en.wikipedia.org/wiki/SOLID