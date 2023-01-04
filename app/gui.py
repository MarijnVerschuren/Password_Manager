"""
		includes
"""
import dearpygui.dearpygui as gui	# gui library
import sys, os						# py libraries
from core import *					# app core
from lib import *					# c++ library



"""
		globals
"""
resource_folder = os.path.join(root_folder, "resources")



"""
		functionality
"""
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



"""
		gui entry
"""
if __name__ == "__main__":
	if "-help" in sys.argv: print(
			"[-new]\t\tmake new lockbox",
			sep="\n", end="\n\n"
		); exit(0)

	clear_console()
	kwargs = {"new_lockbox": False}
	if "-new" in sys.argv:		kwargs["new_lockbox"] = True
	gui_mode(**kwargs)