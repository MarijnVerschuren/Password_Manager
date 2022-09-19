from lib import *
import sys
import os

root_folder = os.path.dirname(os.path.abspath(__file__))
user_folder = os.path.join(root_folder, "user")
data_folder = os.path.join(root_folder, "data")



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
		self.path =	os.path.join(user_folder, self.name)
		# check if the hash in the user file is the same
		# then load passwords
		if not os.path.exists(self.path):
			clear_print("incorrect lockbox name\n")
			return self.unlock()  # call function recursively
		with open(self.path, "rb") as file:
			pass



def console_mode(new_user: bool) -> None:
	lockbox = Lockbox()
	if new_user:	lockbox.new()					# create account
	else:			lockbox.unlock()				# unlock using name and key

def gui_mode(new_user: bool) -> None:
	pass



if __name__ == "__main__":
	# a = AES()
	#a.EncryptCBC(plain, plainLen, s.add("abcdef"));
	#s = SHA256()
	#print(s("abcdef"))

	if "-help" in sys.argv: print(
			"[-new]\t\tmake new user",
			"[-nogui]\trun password manager in console mode",
			"",
			"(*)\t\t= required",
			"(n,)\t\t= n args",
			sep="\n", end="\n\n"
		); exit(0)

	clear_console()
	args = {"new_user": False}
	if "-new" in sys.argv:		args["new_user"] = True
	if "-nogui" in sys.argv:	console_mode(**args)
	else:						gui_mode(**args)

# https://github.com/hoffstadt/DearPyGui
# https://en.wikipedia.org/wiki/SOLID