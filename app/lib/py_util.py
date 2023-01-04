import threading
import sys, os



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



class CON:
	DEFAULT =	0
	NEGATIVE =	7
	RED =		91
	GREEN =		92
	WARNING =	93
	BLUE =		94
	PURPLE =	95
	CYAN =		96

	@staticmethod
	def get_color(code: int) -> str:
		return f"\033[{code}m"


	@classmethod
	def color(cls: object, msg: str, code: int) -> str:
		return f"{cls.get_color(code)}{msg}{cls.get_color(cls.DEFAULT)}"

	@staticmethod
	def move(x: int, y: int) -> None:
		print(f"\033[{y};{x}H")

	@staticmethod
	def clear() -> None:
		os.system("cls" if os.name in ["nt", "dos"] else "clear")


	@classmethod
	def clear_print(cls: object, *args, **kwargs) -> None:
		cls.clear();
		print(*args, **kwargs)

	@classmethod
	def color_print(cls: object, msg: str, code: int, **kwargs) -> None:
		print(cls.color(msg, code), **kwargs)