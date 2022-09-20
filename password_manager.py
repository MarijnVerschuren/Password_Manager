# gui
import dearpygui.dearpygui as gui
# py libraries
import sys, os
# c++ library
from lib import *

root_folder = os.path.dirname(os.path.abspath(__file__))
data_folder = os.path.join(root_folder, "data")



class Lockbox:
	def __init__(self) -> None:
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
			return "lockbox already exists"
		open(self.path, "x").close()

	def unlock(self, name, key) -> str or None:
		self.name =	name;
		self.key = key
		self.path =	os.path.join(data_folder, self.name)
		if not os.path.exists(self.path):
			return "lockbox name does not exist"



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
		gui.add_input_text(label="lockbox name",		tag="new_lockbox_name")
		gui.add_input_text(label="lockbox key",			tag="new_lockbox_key",			password=True)
		gui.add_input_text(label="repeat lockbox key",	tag="repeat_new_lockbox_key",	password=True)
		gui.add_button(label="New", callback=new_callback)
		gui.add_text(tag="new_lockbox_error")
	gui.create_viewport(title="New lockbox", width=600, height=200)


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
		gui.add_input_text(label="lockbox name",	tag="unlock_lockbox_name")
		gui.add_input_text(label="lockbox key",		tag="unlock_lockbox_key",	password=True)
		gui.add_button(label="Unlock", callback=unlock_callback)
		gui.add_text(tag="unlock_lockbox_error")
	gui.create_viewport(title="Unlock lockbox", width=600, height=200)



	gui.setup_dearpygui()
	gui.show_viewport()

	if new_lockbox is None:
		pass

	if new_lockbox:
		gui.configure_item("unlock_lockbox", show=False)
		gui.set_primary_window("new_lockbox", True)
	else:
		gui.configure_item("new_lockbox", show=False)
		gui.set_primary_window("unlock_lockbox", True)


	gui.start_dearpygui()
	gui.destroy_context()




if __name__ == "__main__":
	# a = AES()
	#a.EncryptCBC(plain, plainLen, s.add("abcdef"));
	#s = SHA256()
	#print(s("abcdef"))

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