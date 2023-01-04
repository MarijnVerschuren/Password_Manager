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
	# globals
	"root_folder",
	"data_folder",
	# instances
	"logger",
	# functionality
	"Lockbox",
	"new_lockbox",
	"unlock_lockbox"
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
		self.__logger_func =	func
		self.__error =			None

	def catch(self: object, func: callable) -> callable:
		def wrapper(*args, **kwargs) -> any:
			self.__error = None
			try:					return func(*args, **kwargs)
			except Exception as e:	self.__error = e; self.log()
			return None
		wrapper.__name__ = func.__name__  # to prevent overwriting existing endpoints for "wrapper"
		return wrapper

	def set_func(self: object, logger_func: callable) -> None:
		logger.__logger_func = logger_func

	def log(self: object) -> None:
		self.logger_func(f"Exception occurred: {self.error}")

	@property
	def logger_func(self: object) -> callable:
		return self.__logger_func
	@property
	def error(self: object) -> str:
		return self.__error

def print_error(msg: str) -> None:
	CON.color_print(msg, CON.RED)



"""
		class instances
"""
logger =	Logger(print_error)  # default logger (print in red)
sha3_512 =	SHA3(SHA3.bits512)



"""
		functionality
"""
class Chunk:
	def __init__(
		self: object
	) -> None:
		pass


class Lockbox:
	def __init__(
		self:	object,
		name:	str,
		path:	str,
		key:	str
	) -> None:
		self.__name = name
		self.__path = path
		self.__key = key
		self.__chunk_mapping = None
		self.map_chunks()  # start "lazy" loading file

	@logger.catch
	def map_chunks(self: object) -> None:
		pass

	@logger.catch
	def read_chunk(self: object, chunk_id: int) -> Chunk:
		pass
	# maybe add generator

	@logger.catch
	def add_chunk(self: object, chunk: Chunk) -> None:
		pass

	@logger.catch
	def delete_chunk(self: object, chunk_id: int) -> None:
		pass


@logger.catch
def new_lockbox(name: str, key_0: str, key_1: str) -> Lockbox:
	if key_0 != key_1:							raise invalid_password("passwords do not match")
	path =	os.path.join(data_folder, name)
	if os.path.exists(path):
		if os.path.getsize(path) >= 128:		raise invalid_lockbox_name("name taken")
		# if the file size is smaller than 128B the file is not taken as it has no header
	salt = os.urandom(64)
	sha3_512.add(key.encode("ascii") + salt)
	with open(path, "wb") as ofile:				# overwrite
		ofile.write(salt)						# 64B
		ofile.write(sha3_512.raw_hash)			# 64B
		ofile.close()
	sha3_512.reset()
	return Lockbox(name, path, key_0)

@logger.catch
def unlock_lockbox(name: str, key: str) -> Lockbox:
	path =	os.path.join(data_folder, name)
	if not os.path.exists(path):				raise invalid_lockbox_name("name not found")
	with open(path, "rb") as ifile:
		salt = ifile.read(64)
		_hash = ifile.read(64)
		ifile.close()
	sha3_512.add(key.encode("ascii") + salt)
	if sha3_512.raw_hash != _hash:				raise invalid_password("password incorrect")
	sha3_512.reset()
	return Lockbox(name, path, key)



"""
		TODO
"""
# https://github.com/hoffstadt/DearPyGui
# https://en.wikipedia.org/wiki/SOLID