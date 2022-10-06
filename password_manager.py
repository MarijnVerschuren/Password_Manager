# gui
import dearpygui.dearpygui as gui
# py libraries
import sys, os
# c++ library
from lib import *

root_folder = os.path.dirname(os.path.abspath(__file__))
resource_folder = os.path.join(root_folder, "resources")
data_folder = os.path.join(root_folder, "data")



class Lockbox:
	def __init__(self) -> None:
		self.sha3_512 = SHA3(SHA3.bits512)
		self.name = None
		self.path = None
		self.key = None


	def new(self, name, key_0, key_1) -> str or None:
		self.name = name
		if key_0 != key_1:
			return "keys do not match"
		self.key = key_0;
		self.path =	os.path.join(data_folder, self.name)
		if os.path.exists(self.path):
			if os.path.getsize(self.path) >= 128:
				return "lockbox already exists"
			# if the file size is smaller than 128B the file is not taken as it has no header
		salt = os.urandom(64)
		self.sha3_512.add(self.key.encode("ascii") + salt)
		with open(self.path, "wb") as ofile:		# overwrite
			ofile.write(salt)						# 64B
			ofile.write(self.sha3_512.raw_hash)		# 64B
			ofile.close()
		self.sha3_512.reset()


	def unlock(self, name, key) -> str or None:
		self.name =	name;
		self.key = key
		self.path =	os.path.join(data_folder, self.name)
		if not os.path.exists(self.path):
			return "lockbox name does not exist"
		with open(self.path, "rb") as ifile:
			salt = ifile.read(64)
			_hash = ifile.read(64)
			ifile.close()
		self.sha3_512.add(key.encode("ascii") + salt)
		if self.sha3_512.raw_hash != _hash: return "password incorrect"
		self.sha3_512.reset()
		return read_blocks()


	def read_blocks(self) -> str or None:
		pass

# TODO: continue after success

# --------------------------------------------------------------------------------------------- #
#			UI																					|
# --------------------------------------------------------------------------------------------- #
def console_mode(new_lockbox: bool) -> None:
	lockbox = Lockbox()
	error = True
	while error is not None:
		if new_lockbox:	error = lockbox.new(
			input("lockbox name: "),
			getpass("lockbox key: "),
			getpass("repeat lockbox key: ")
		)				# create lockbox
		else: error = lockbox.unlock(
			input("lockbox name: "),
			getpass("lockbox key: ")
		)				# unlock using lockbox name and key
		if error: clear_print(chs.apply(f"[{error}]", chs.RED))


def gui_mode(new_lockbox: bool = None) -> None:
	lockbox = Lockbox()
	gui.create_context()

	with gui.font_registry():
		helvetica = gui.add_font(os.path.join(resource_folder, "font.otf"), 17)

	# Create new lockbox
	def new_callback(sender, app_data):
		error = True
		while error is not None:
			error = lockbox.new(
				gui.get_value("new_lockbox_name"),
				gui.get_value("new_lockbox_key"),
				gui.get_value("repeat_new_lockbox_key")
			)
			if error: gui.set_value("new_lockbox_error", error)

	with gui.window(label="New lockbox", tag="new_lockbox"):
		gui.bind_font(helvetica)
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_text		("lockbox name:")
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_input_text	(tag="new_lockbox_name")
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_text		("lockbox key:")
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_input_text	(tag="new_lockbox_key",			password=True)
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_text		("repeat lockbox key:")
		with gui.group(horizontal=True):	gui.add_spacer(width=50, height=25);	gui.add_input_text	(tag="repeat_new_lockbox_key",	password=True)
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_button		(label="New",			callback=new_callback);		gui.add_spacer(width=82);	gui.add_text(tag="new_lockbox_error")


	# Unlock lockbox
	def unlock_callback(sender, app_data):
		error = True
		while error is not None:
			error = lockbox.unlock(
				gui.get_value("unlock_lockbox_name"),
				gui.get_value("unlock_lockbox_key")
			)
			if error: gui.set_value("unlock_lockbox_error", error)

	with gui.window(label="Unlock lockbox", tag="unlock_lockbox"):
		gui.bind_font(helvetica)
		gui.add_spacer(height=20)
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_text		("lockbox name:")
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_input_text	(tag="unlock_lockbox_name")
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_text		("lockbox key:")
		with gui.group(horizontal=True):	gui.add_spacer(width=50, height=25);	gui.add_input_text	(tag="unlock_lockbox_key",	password=True)
		with gui.group(horizontal=True):	gui.add_spacer(width=50);				gui.add_button		(label="Unlock", callback=unlock_callback);		gui.add_spacer(width=2);	gui.add_text(tag="unlock_lockbox_error")



	if new_lockbox is None:
		pass  # show choice between new and unlock

	if new_lockbox:
		gui.create_viewport(title="New lockbox", width=400, height=200, resizable=False)
		gui.configure_item("unlock_lockbox", show=False)
		gui.set_primary_window("new_lockbox", True)
	else:
		gui.create_viewport(title="Unlock lockbox", width=400, height=150, resizable=False)
		gui.configure_item("new_lockbox", show=False)
		gui.set_primary_window("unlock_lockbox", True)



	gui.setup_dearpygui()
	gui.show_viewport()
	gui.start_dearpygui()
	gui.destroy_context()




if __name__ == "__main__":
	if "-help" in sys.argv: print(
			"[-new]\t\tmake new lockbox",
			"[-nogui]\trun password manager in console mode",
			"",
			"(*)\t\t= required",
			"(n,)\t\t= n args",
			sep="\n", end="\n\n"
		); exit(0)

	clear_console()
	args = {"new_lockbox": False}
	if "-new" in sys.argv:		args["new_lockbox"] = True
	if "-nogui" in sys.argv:	console_mode(**args)
	else:						gui_mode(**args)

# https://github.com/hoffstadt/DearPyGui
# https://en.wikipedia.org/wiki/SOLID