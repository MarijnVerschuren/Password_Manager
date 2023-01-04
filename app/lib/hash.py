from .py_lib import SHA256
from .py_lib import SHA3 as _SHA3



class SHA3(_SHA3):
	def __init__(self, hash_type: _SHA3.SHA3_type = _SHA3.bits512) -> None:
		super(SHA3, self).__init__(hash_type)