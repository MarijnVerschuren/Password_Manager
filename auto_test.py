import os



def SHA256_stream_test(test_count: int = 0xff) -> None:
	tests = [
		[("a",),		"ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"],	# hash of "a"
		[("b",),		"fb8e20fc2e4c3f248c60c39bd652f3c1347298bb977b8b4d5903b85055620603"],	# hash of "ab"
		[("c",),		"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"],	# hash of "abc"
		[("d",),		"88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589"],	# hash of "abcd"
		[("e",),		"36bbe50ed96841d10443bcb670d6554f0a34b761be67ec9c4a8ad2c0c44ca42c"],	# hash of "abcde"
		[("f",),		"bef57ec7f53a6d40beb640a780a639c83bc29ac8a9816f1fc6c5c6dcd93c4721"]		# hash of "abcdef"
	]
	test_count = min(len(tests), test_count)	# clamp to the max test count
	tests = tests[0:test_count]					# set test count
	results = []

	s = SHA256()
	fail = False
	for args, expect in tests:
		s.add(*args)
		result = s.raw_hash.hex()
		if result != expect: fail = True
		results.append([args, expect, result])

	print("SHA256 (stream):\t", end="")
	if fail:
		print(chs.apply("[FAULT]", chs.RED))
		for args, expect, result in results:
			print(
				f"args:\t\t\t{chs.apply(f'{args}', chs.NEGATIVE)}",
				f"expected:\t\t{chs.apply(f'{expect}', chs.NEGATIVE)}",
				f"got:\t\t\t{chs.apply(f'{result}', chs.NEGATIVE)}",
				sep="\n", end="\n\n"
			)
		return
	print(chs.apply("[OK]", chs.GREEN));



def AES_CBC_encrypt_test(test_count: int = 0xff) -> None:
	tests = [
		[("a_",),		"ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"],
		[("b",),		"fb8e20fc2e4c3f248c60c39bd652f3c1347298bb977b8b4d5903b85055620603"],
		[("c",),		"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"],
		[("d",),		"88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589"],
		[("e",),		"36bbe50ed96841d10443bcb670d6554f0a34b761be67ec9c4a8ad2c0c44ca42c"],
		[("f",),		"bef57ec7f53a6d40beb640a780a639c83bc29ac8a9816f1fc6c5c6dcd93c4721"]
	]
	test_count = min(len(tests), test_count)	# clamp to the max test count
	tests = tests[0:test_count]					# set test count
	results = []

	a = AES()
	print(a.CBC_encrypt(b"abc", os.urandom(32), os.urandom(16)))



if __name__ == "__main__":
	# compile the library
	os.system("cmplib.bat")
	from lib import *  # include after compile to prevent sharing violation

	print("\n--------------------------------------------------", end="\n\n")

	#SHA256_stream_test()
	#AES_CBC_encrypt_test()
	print(func(b"abc"))

	print("\n--------------------------------------------------", end="\n\n")