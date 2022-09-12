import os



def SHA256_test() -> None:
	s = SHA256()
	s.add("abcdef")
	actual = s.raw_hash.hex()
	expected = "bef57ec7f53a6d40beb640a780a639c83bc29ac8a9816f1fc6c5c6dcd93c4721"

	print("SHA256:\t\t", end="")
	if expected == actual:
		print(chs.apply("[OK]", chs.GREEN)); return
	print(chs.apply("[FAULT]", chs.RED))
	print(
		f"expected:\t{chs.apply(f'{expected}', chs.NEGATIVE)}",
		f"got:\t\t{chs.apply(f'{actual}', chs.NEGATIVE)}",
		sep="\n", end="\n\n"
	)




if __name__ == "__main__":
	# compile the library
	os.system("cmplib.bat")
	from lib import *  # include after compile to prevent sharing violation

	print("\n--------------------------------------------------\n")

	SHA256_test()

	print("\n--------------------------------------------------")