import pyautogui
import sys, os
import time
import json



# utils
def print_error(results) -> None:
	for args, expect, result in results:
		modifier = chs.END
		if result != expect: modifier = chs.NEGATIVE
		print(
			f"args:\t\t\t{chs.apply(f'{args}', modifier)}",
			f"expected:\t\t{chs.apply(f'{expect}', modifier)}",
			f"got:\t\t\t{chs.apply(f'{result}', modifier)}",
			sep="\n", end="\n\n"
		)


# test formats
def SHA_stream_test(hash_obj: object, tests: list, prompt: str) -> None:
	results = []

	fail = False
	for args, expect in tests:
		hash_obj.add(*args)
		result = hash_obj.raw_hash.hex()
		if result != expect: fail = True
		results.append([args, expect, result])

	print(f"{prompt}:\t", end="")
	if fail:
		print(chs.apply("[FAULT]", chs.RED))
		print_error(results)
		return
	print(chs.apply("[OK]", chs.GREEN));



def AES_test(encrypt_function: callable, decrypt_function: callable, tests: list, prompt: str) -> None:
	# convert all hex strings back to bytes to use as args
	formatted_tests = [[], []]
	for args, answer in tests[0]:  # encrypt
		formatted_args = [args[0]]  # first arg is just text in encrypt test
		for arg in args[1:]: formatted_args.append(bytes.fromhex(arg))
		formatted_tests[0].append([formatted_args, bytes.fromhex(answer)])
	for args, answer in tests[1]:  # decrypt
		formatted_args = []
		for arg in args: formatted_args.append(bytes.fromhex(arg))
		formatted_tests[1].append([formatted_args, bytes.fromhex(answer)])
	tests = formatted_tests

	results = [[], []]

	fail = 0
	for args, expect in tests[0]:
		result = encrypt_function(*args)
		if result != expect: fail |= 1
		results[0].append([args, expect, result])

	for args, expect in tests[1]:
		result = decrypt_function(*args)
		if result != expect: fail |= 2
		results[1].append([args, expect, result])

	print(f"{prompt}:\t\t", end="")
	if fail:
		if fail == 1: print(chs.apply("[ENCRYPTION FAULT]", chs.RED))
		if fail == 2: print(chs.apply("[DECRYPTION FAULT]", chs.RED))
		if fail == 3: print(chs.apply("[GENERAL FAULT]", chs.RED))

		if fail & 1:
			print(chs.apply("====================[ENCRYPT]=====================", chs.CYAN))
			print_error(results[0])
		if fail & 2:
			print(chs.apply("====================[DECRYPT]=====================", chs.CYAN))
			print_error(results[1])
		return
	print(chs.apply("[OK]", chs.GREEN));



if __name__ == "__main__":
	enable_halting_functions = False
	if "-help" in sys.argv: print(
			"[-no-compile]\t\t\tdont compile before test",
			"[-halting-function-test]\tinclude halting functions in test",
			sep="\n", end="\n\n"
		); exit(0)
	if not "-no-compile" in sys.argv: os.system("cmplib.bat")  # compile the library
	if "-halting-function-test" in sys.argv: enable_halting_functions = True

	from lib import *  # include after compile to prevent sharing violation


	# hash
	sha256 = SHA256()
	sha3_224 = SHA3(SHA3.bits224)
	sha3_256 = SHA3(SHA3.bits256)
	sha3_384 = SHA3(SHA3.bits384)
	sha3_512 = SHA3(SHA3.bits512)

	# aes
	aes = AES()

	# read tests
	with open("test_data.json", "r") as i_file:
		tests = json.load(i_file)
		i_file.close()

	print("\n--------------------------------------------------", end="\n\n")

	SHA_stream_test(sha256, tests["SHA256_stream_tests"], "SHA256 (stream)")
	SHA_stream_test(sha3_224, tests["SHA3_224_stream_tests"], "SHA3-224 (stream)")
	SHA_stream_test(sha3_256, tests["SHA3_256_stream_tests"], "SHA3-256 (stream)")
	SHA_stream_test(sha3_384, tests["SHA3_384_stream_tests"], "SHA3-384 (stream)")
	SHA_stream_test(sha3_512, tests["SHA3_512_stream_tests"], "SHA3-512 (stream)")
	AES_test(aes.encrypt_ECB, aes.decrypt_ECB, tests["AES_ECB_tests"], "AES_ECB")
	AES_test(aes.encrypt_CBC, aes.decrypt_CBC, tests["AES_CBC_tests"], "AES_CBC")
	AES_test(aes.encrypt_CFB, aes.decrypt_CFB, tests["AES_CFB_tests"], "AES_CFB")
	if enable_halting_functions:
		t = thread(target=getpass, args=("getpass test: ", ))
		t.start()
		pyautogui.write("abcd")
		print(t.join())

	print("\n--------------------------------------------------", end="\n\n")

	# https://pybind11.readthedocs.io/en/stable/advanced/functions.html
	# TODO: load tests from json file

# TODO:
# add SHA3-512, CRC64, CRC(n) check
# sha3-512:	https://www.google.com/search?q=c%2B%2B+sha3-512+library&oq=c%2B%2B+sha3-512+library&aqs=chrome..69i57j33i160.9059j0j7&sourceid=chrome&ie=UTF-8
# OR
# sha512:
# https://rweather.github.io/arduinolibs/Hash_8h_source.html
# https://rweather.github.io/arduinolibs/SHA512_8h_source.html
# https://rweather.github.io/arduinolibs/SHA512_8cpp_source.html

# crc (own implementation)

# youtube.com/watch?v=_5T70cAXDJ0