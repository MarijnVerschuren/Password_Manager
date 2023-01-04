"""
		includes
"""
import sys, os						# py libraries
from core import *					# app core
from lib import *					# c++ library



"""
		functionality
"""
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



"""
		cli entry
"""
if __name__ == "__main__":
	if "-help" in sys.argv: print(
			"[-new]\t\tmake new lockbox",
			sep="\n", end="\n\n"
		); exit(0)

	clear_console()
	kwargs = {"new_lockbox": False}
	if "-new" in sys.argv:		kwargs["new_lockbox"] = True
	console_mode(**kwargs)