"""
		includes
"""
import sys, os						# py libraries
from pynput import keyboard
import pyautogui
from core import *					# app core
from lib import *					# c++ library



"""
		functionality
"""
def console_select(*choices) -> int:
	choice_count = len(choices)
	current = 0;
	def update() -> None:
		CON.move(0, 0)
		for choice in range(choice_count): CON.color_print(choices[choice], CON.NEGATIVE if choice == current else CON.DEFAULT, end=" ")
	CON.clear();
	with keyboard.Events() as events:
		pyautogui.press("left")  # initialize to default value (0)
		for event in events:
			if event.key == keyboard.Key.left:		current = max(current - 1, 0)
			elif event.key == keyboard.Key.right:	current = min(current + 1, choice_count - 1)
			if event.key == keyboard.Key.enter:		input(); return current
			update()



def run() -> None:
	choice = console_select("new", "unlock")
	CON.clear()

	lockbox = unlock_lockbox(
		input("lockbox name: "),
		getpass("lockbox key: ")
	) if choice else new_lockbox(
		input("lockbox name: "),
		getpass("lockbox key: "),
		getpass("repeat lockbox key: ")
	)



"""
		cli entry
"""
if __name__ == "__main__":
	if "-help" in sys.argv: print(
			sep="\n", end="\n\n"
		); exit(0)

	CON.clear()
	run()



"""
		TODO
"""
# redo gui funcions