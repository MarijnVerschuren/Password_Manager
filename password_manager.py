from lib import *
import sys
import os

root_folder = os.path.dirname(os.path.abspath(__file__))
user_folder = os.path.join(root_folder, "user")
data_folder = os.path.join(root_folder, "data")


"""
import base64
import hashlib
from Crypto import Random
from Crypto.Cipher import AES

class AESCipher(object):

    def __init__(self, key): 
        self.bs = AES.block_size
        self.key = hashlib.sha256(key.encode()).digest()

    def encrypt(self, raw):
        raw = self._pad(raw)
        iv = Random.new().read(AES.block_size)
        cipher = AES.new(self.key, AES.MODE_CBC, iv)
        return base64.b64encode(iv + cipher.encrypt(raw.encode()))

    def decrypt(self, enc):
        enc = base64.b64decode(enc)
        iv = enc[:AES.block_size]
        cipher = AES.new(self.key, AES.MODE_CBC, iv)
        return self._unpad(cipher.decrypt(enc[AES.block_size:])).decode('utf-8')

    def _pad(self, s):
        return s + (self.bs - len(s) % self.bs) * chr(self.bs - len(s) % self.bs)

    @staticmethod
    def _unpad(s):
    	# check if it is realy padding and not message (by checking if every padding byte is the same)
        return s[:-ord(s[len(s)-1:])]
"""


class Lockbox:
	def __init__(self) -> None:
		self.name = None
		self.path = None
		self.key = None

	def new(self) -> None:
		self.name = input("lockbox name: ")
		while True:
			pass_0 = getpass("lockbox key: ")
			pass_1 = getpass("repeat lockbox key: ")
			if pass_0 != pass_1:
				clear_console()
				print("keys do not match try again:")
				continue
			self.key = pass_0; break
		# logged in
		# encrypted file with passwords


	def unlock(self) -> None:  # TODO
		self.name =	input("lockbox name: ")
		self.key =	getpass("lockbox key: ")
		self.path =	os.path.join(user_folder, lockbox_name)
		# check if the hash in the user file is the same
		# then load passwords
		with open(path, "rb") as file:
			




def console_mode(new_user: bool) -> None:
	lockbox = Lockbox()
	if new_user:	lockbox.new()					# create account
	else:			lockbox.unlock()				# unlock using name and key

def gui_mode(new_user: bool) -> None:
	pass



if __name__ == "__main__":
	if "-help" in sys.argv: print(
			"[-new]\t\tmake new user",
			"[-nogui]\trun password manager in console mode",
			"",
			"(*)\t\t= required",
			"(n,)\t\t= n args",
			sep="\n", end="\n\n"
		); exit(0)

	args = {"new_user": False}

	if "-new" in sys.argv:		args["new_user"] = True
	if "-nogui" in sys.argv:	console_mode(**args)
	else:						gui_mode(**args)
# https://github.com/hoffstadt/DearPyGui
# https://en.wikipedia.org/wiki/SOLID


# TODO:
# add SHA3-512, SHA256, CRC64, CRC(n) check