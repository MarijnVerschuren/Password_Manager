import sys, os


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



	# HASH_TESTS
	sha256 = SHA256()
	SHA256_stream_tests = [
		[("a",),	"ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"],	# hash of "a"
		[("b",),	"fb8e20fc2e4c3f248c60c39bd652f3c1347298bb977b8b4d5903b85055620603"],	# hash of "ab"
		[("c",),	"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"],	# hash of "abc"
		[("d",),	"88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589"],	# hash of "abcd"
		[("e",),	"36bbe50ed96841d10443bcb670d6554f0a34b761be67ec9c4a8ad2c0c44ca42c"],	# hash of "abcde"
		[("f",),	"bef57ec7f53a6d40beb640a780a639c83bc29ac8a9816f1fc6c5c6dcd93c4721"]		# hash of "abcdef"
	]

	sha3_224 = SHA3(SHA3.bits224)
	SHA3_224_stream_tests = [
		[("a",),	"9e86ff69557ca95f405f081269685b38e3a819b309ee942f482b6a8b"],	# hash of "a"
		[("b",),	"09d27a15bcbab5da828d84dbd66062e5d37049f9b165a65dc581e853"],	# hash of "ab"
		[("c",),	"e642824c3f8cf24ad09234ee7d3c766fc9a3a5168d0c94ad73b46fdf"],	# hash of "abc"
		[("d",),	"dd886b5fd8421fb3871d24e39e53967ce4fc80dd348bedbea0109c0e"],	# hash of "abcd"
		[("e",),	"6acfaab70afd8439cea3616b41088bd81c939b272548f6409cf30e57"],	# hash of "abcde"
		[("f",),	"ceb3f4cd85af081120bf69ecf76bf61232bd5d810866f0eca3c8907d"]		# hash of "abcdef"
	]

	sha3_256 = SHA3(SHA3.bits256)
	SHA3_256_stream_tests = [
		[("a",),	"80084bf2fba02475726feb2cab2d8215eab14bc6bdd8bfb2c8151257032ecd8b"],	# hash of "a"
		[("b",),	"5c828b33397f4762922e39a60c35699d2550466a52dd15ed44da37eb0bdc61e6"],	# hash of "ab"
		[("c",),	"3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532"],	# hash of "abc"
		[("d",),	"6f6f129471590d2c91804c812b5750cd44cbdfb7238541c451e1ea2bc0193177"],	# hash of "abcd"
		[("e",),	"d716ec61e18904a8f58679b71cb065d4d5db72e0e0c3f155a4feff7add0e58eb"],	# hash of "abcde"
		[("f",),	"59890c1d183aa279505750422e6384ccb1499c793872d6f31bb3bcaa4bc9f5a5"]		# hash of "abcdef"
	]

	sha3_384 = SHA3(SHA3.bits384)
	SHA3_384_stream_tests = [
		[("a",),	"1815f774f320491b48569efec794d249eeb59aae46d22bf77dafe25c5edc28d7ea44f93ee1234aa88f61c91912a4ccd9"],	# hash of "a"
		[("b",),	"dc30f83fefe3396fa0bd9709bcad28394386aa4e28ae881dc6617b361b16b969fb6a50a109068f13127b6deffbc82d4b"],	# hash of "ab"
		[("c",),	"ec01498288516fc926459f58e2c6ad8df9b473cb0fc08c2596da7cf0e49be4b298d88cea927ac7f539f1edf228376d25"],	# hash of "abc"
		[("d",),	"5af1d89732d4d10cc6e92a36756f68ecfbf7ae4d14ed4523f68fc304cccfa5b0bba01c80d0d9b67f9163a5c211cfd65b"],	# hash of "abcd"
		[("e",),	"348494236b82edda7602c78ba67fc3838e427c63c23e2c9d9aa5ea6354218a3c2ca564679acabf3ac6bf5378047691c4"],	# hash of "abcde"
		[("f",),	"d77460b0ce6109168480e279a81af32facb689ab96e22623f0122ff3a10ead263db6607f83876a843d3264dc2a863805"]		# hash of "abcdef"
	]

	sha3_512 = SHA3(SHA3.bits512)
	SHA3_512_stream_tests = [
		[("a",),	"697f2d856172cb8309d6b8b97dac4de344b549d4dee61edfb4962d8698b7fa803f4f93ff24393586e28b5b957ac3d1d369420ce53332712f997bd336d09ab02a"],	# hash of "a"
		[("b",),	"01c87b5e8f094d8725ed47be35430de40f6ab6bd7c6641a4ecf0d046c55cb468453796bb61724306a5fb3d90fbe3726a970e5630ae6a9cf9f30d2aa062a0175e"],	# hash of "ab"
		[("c",),	"b751850b1a57168a5693cd924b6b096e08f621827444f70d884f5d0240d2712e10e116e9192af3c91a7ec57647e3934057340b4cf408d5a56592f8274eec53f0"],	# hash of "abc"
		[("d",),	"6eb7b86765bf96a8467b72401231539cbb830f6c64120954c4567272f613f1364d6a80084234fa3400d306b9f5e10c341bbdc5894d9b484a8c7deea9cbe4e265"],	# hash of "abcd"
		[("e",),	"1d7c3aa6ee17da5f4aeb78be968aa38476dbee54842e1ae2856f4c9a5cd04d45dc75c2902182b07c130ed582d476995b502b8777ccf69f60574471600386639b"],	# hash of "abcde"
		[("f",),	"01309a45c57cd7faef9ee6bb95fed29e5e2e0312af12a95fffeee340e5e5948b4652d26ae4b75976a53cc1612141af6e24df36517a61f46a1a05f59cf667046a"]		# hash of "abcdef"
	]


	# ENCRYPTION_TESTS
	aes = AES()
	# AES_ECB_TEST
	ECB_key = b'GE\x97\xef\x19\xac=\x9f\xd5\xe3\x0cr\xad\x12\xff\x002\xd7\x19\xe1\xb7\xc5p\xe6\xde\x93\x91\xd46_\xa91'
	ECB_tests = [
		[
			[("a", ECB_key),	b'\xa7\xe5\x99\x92\x9eg2L \x7f\xb6wn$\x95\x7f'																		],
			[("b", ECB_key),	b'\x1fs\x8d\xaa\x166 \xc8\xd6\xf4\xa0s\x11\xe9\xa4\xaf'																],
			[("c", ECB_key),	b'\x86nm\xb8\x9am\xae\x10\xdc\xce\xa7|\x1avXI'																		],
			[("d", ECB_key),	b'A\xdd\x8d\x1fV\x02\xb8q\xbaJP\x94\xba\x8a1\xa4'																	],
			[("e", ECB_key),	b'X"\xaa\x0e\x04,\x93`y\xaf[\xe2\x82\xc2\xaa}'																		],
			[("f", ECB_key),	b'_\x04Um\xcawEm\xe7\xec;*R\x89\x9a\xb9'																			]
		],
		[
			[(b'\xa7\xe5\x99\x92\x9eg2L \x7f\xb6wn$\x95\x7f',			ECB_key),	b'a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\x1fs\x8d\xaa\x166 \xc8\xd6\xf4\xa0s\x11\xe9\xa4\xaf',	ECB_key),	b'b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\x86nm\xb8\x9am\xae\x10\xdc\xce\xa7|\x1avXI',			ECB_key),	b'c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'A\xdd\x8d\x1fV\x02\xb8q\xbaJP\x94\xba\x8a1\xa4',		ECB_key),	b'd\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'X"\xaa\x0e\x04,\x93`y\xaf[\xe2\x82\xc2\xaa}',			ECB_key),	b'e\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'_\x04Um\xcawEm\xe7\xec;*R\x89\x9a\xb9',					ECB_key),	b'f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00']
		]
	]

	CBC_key = b'\x1b\xed\x96\xe3\x99\xaa\xbd@\xf2\xac1\x85\xe0\xa5\xcf\xe0\xda".r\xe8\xf2\x92\x9b\xc7\xdb\xf2Y\x0f\xc2l:'
	CBC_iv = b'\xca\xb7\xc1\xf1PVc\x9e\xd6\xd98\xc7WC\x13D'
	CBC_tests = [
		[
			[("a", CBC_key, CBC_iv),	b'\n\x00CD\xc9}Zuo/"ddX\xa1\x05'																						],
			[("b", CBC_key, CBC_iv),	b'\x152^\xb3\xa0Z\\\xd8\xd9\xcb\xd0\xb3t\xf4-y'																			],
			[("c", CBC_key, CBC_iv),	b'\x08\xa2ID\x81\xab\xbf9\xaevq\x99I\\\xa3\xcf'																			],
			[("d", CBC_key, CBC_iv),	b'\xf4:(l\xb7\xde\xc8\x94\x95\xc2;`3\xa4\x9c%'																			],
			[("e", CBC_key, CBC_iv),	b'\xa2\xd4\x03\xce\xe6\x1b\xd2\xfe\xc2\x15U\x1b?\x9e\xd0,'																],
			[("f", CBC_key, CBC_iv),	b'\x0e\x0cl.\x07Z\xf8\x87\x87\x0c\x85\xb3\xe8O\x8b\x07'																	]
		],
		[
			[(b'\n\x00CD\xc9}Zuo/"ddX\xa1\x05',								CBC_key, CBC_iv),	b'a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\x152^\xb3\xa0Z\\\xd8\xd9\xcb\xd0\xb3t\xf4-y',				CBC_key, CBC_iv),	b'b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\x08\xa2ID\x81\xab\xbf9\xaevq\x99I\\\xa3\xcf',				CBC_key, CBC_iv),	b'c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\xf4:(l\xb7\xde\xc8\x94\x95\xc2;`3\xa4\x9c%',				CBC_key, CBC_iv),	b'd\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\xa2\xd4\x03\xce\xe6\x1b\xd2\xfe\xc2\x15U\x1b?\x9e\xd0,',	CBC_key, CBC_iv),	b'e\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'\x0e\x0cl.\x07Z\xf8\x87\x87\x0c\x85\xb3\xe8O\x8b\x07',		CBC_key, CBC_iv),	b'f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00']
		]
	]

	CFB_key = b'Z%\xe0_x\xca\xecw.\xa8\xfe\xdd\x08JL\x84\xc1\xf2\xf3\x99\xe7\xad\x02di8\xac/\xf8\x8d\x0e\xc0'
	CFB_iv = b'<@\xdbu5D\x8b)v\xa2\xc9\x02\xbd \xdd['
	CFB_tests = [
		[
			[("a", CFB_key, CFB_iv),	b'Ac\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N'																],
			[("b", CFB_key, CFB_iv),	b'Bc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N'																],
			[("c", CFB_key, CFB_iv),	b'Cc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N'																],
			[("d", CFB_key, CFB_iv),	b'Dc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N'																],
			[("e", CFB_key, CFB_iv),	b'Ec\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N'																],
			[("f", CFB_key, CFB_iv),	b'Fc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N'																]
		],
		[
			[(b'Ac\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N',	CFB_key, CFB_iv),	b'a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'Bc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N',	CFB_key, CFB_iv),	b'b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'Cc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N',	CFB_key, CFB_iv),	b'c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'Dc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N',	CFB_key, CFB_iv),	b'd\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'Ec\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N',	CFB_key, CFB_iv),	b'e\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'],
			[(b'Fc\xc3\x18D\xce\xcd\x84\x14\x83%Vt}\xc2N',	CFB_key, CFB_iv),	b'f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00']
		]
	]

	print("\n--------------------------------------------------", end="\n\n")

	SHA_stream_test(sha256, SHA256_stream_tests, "SHA256 (stream)")
	SHA_stream_test(sha3_224, SHA3_224_stream_tests, "SHA3-224 (stream)")
	SHA_stream_test(sha3_256, SHA3_256_stream_tests, "SHA3-256 (stream)")
	SHA_stream_test(sha3_384, SHA3_384_stream_tests, "SHA3-384 (stream)")
	SHA_stream_test(sha3_512, SHA3_512_stream_tests, "SHA3-512 (stream)")
	AES_test(aes.encrypt_ECB, aes.decrypt_ECB, ECB_tests, "AES_ECB")
	AES_test(aes.encrypt_CBC, aes.decrypt_CBC, CBC_tests, "AES_CBC")
	AES_test(aes.encrypt_CFB, aes.decrypt_CFB, CFB_tests, "AES_CFB")
	if enable_halting_functions:
		print(getpass("getpass test: "))

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