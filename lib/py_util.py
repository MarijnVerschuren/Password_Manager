import threading
import os



class thread(threading.Thread):
	def __init__(self, *args, **kwargs):
		super(thread, self).__init__(*args, **kwargs)
		self._return = None

	def run(self):
		if self._target is not None:
			self._return = self._target(*self._args, **self._kwargs)

	def join(self):
		super(thread, self).join()
		return self._return



class chs:  # (char style) class acts like a namespace
	END =		0
	NEGATIVE =	7
	RED =		91
	GREEN =		92
	WARNING =	93
	BLUE =		94
	PURPLE =	95
	CYAN =		96

	@staticmethod
	def get(code: int) -> str:
		return f"\033[{code}m"

	@classmethod
	def apply(cls, msg: str, code) -> str:
		return f"{cls.get(code)}{msg}{cls.get(cls.END)}"
# TODO: maybe move this to c++


clear_console = lambda: os.system("cls" if os.name in ["nt", "dos"] else "clear")

def clear_print(*args, **kwargs) -> None:
	clear_console();
	print(*args, **kwargs)