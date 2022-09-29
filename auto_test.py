import functools
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
			f"args:\t\t{chs.apply(f'{args}', modifier)}",
			f"expected:\t{chs.apply(f'{expect}', modifier)}",
			f"got:\t\t{chs.apply(f'{result}', modifier)}",
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

	print(prompt, end="")
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

	print(prompt, end="")
	if fail:
		if fail == 1: print(chs.apply("[ENCRYPTION FAULT]", chs.RED))
		if fail == 2: print(chs.apply("[DECRYPTION FAULT]", chs.RED))
		if fail == 3: print(chs.apply("[GENERAL FAULT]", chs.RED))

		if fail & 1:
			if fail & 2: print(chs.apply("                    [ENCRYPT]                     ", chs.PURPLE))
			print_error(results[0])
		if fail & 2:
			if fail & 1: print(chs.apply("                    [DECRYPT]                     ", chs.PURPLE))
			print_error(results[1])
		return
	print(chs.apply("[OK]", chs.GREEN));


def CRC_tablegen_test(func: callable, expect: str, prompt: str) -> None:
	results = []

	result = func().hex()
	results.append([(), expect, result])

	print(prompt, end="")
	if result != expect:
		print(chs.apply("[FAULT]", chs.RED))
		print_error(results)
		return
	print(chs.apply("[OK]", chs.GREEN));


def getpass_test() -> None:
	t = thread(target=getpass, args=("getpass test: ", "*", True)); t.start()
	pyautogui.write("abcd"); pyautogui.press("enter")
	print("getpass:\t\t\t", end="")
	got = t.join()
	if got == "abcd": print(chs.apply("[OK]", chs.GREEN))
	else:
		print(chs.apply("[FAULT]", chs.RED))
		print(chs.apply(f"got:\t\t\t{got}", chs.NEGATIVE))




if __name__ == "__main__":
	# test flags
	no_hash =		False
	no_encryption =	False
	no_crc =		False
	no_console =	False

	if "-help" in sys.argv: print(
			"[-no-compile]\t\t\tdont compile before test",
			"[-no-hash-test]\t\t\tdont test crc",
			"[-no-encryption-test]\t\t\tdont test crc",
			"[-no-crc-test]\t\t\tdont test crc",
			"[-no-console-test]\t\t\tdont test console functions",
			sep="\n", end="\n\n"
		); exit(0)
	if not "-no-compile" in sys.argv: os.system("cmplib.bat")  # compile the library
	# setting test flags
	if "-no-hash-test" in sys.argv:			no_hash =		True
	if "-no-encryption-test" in sys.argv:	no_encryption =	True
	if "-no-crc-test" in sys.argv:			no_crc =		True
	if "-no-console-test" in sys.argv:		no_console =	True

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

	if not no_hash:
		print(chs.apply("======================[HASH]======================", chs.CYAN))
		SHA_stream_test(sha256,															tests["SHA256_stream_tests"],			"SHA256\t\t(stream):\t")
		SHA_stream_test(sha3_224,														tests["SHA3_224_stream_tests"],			"SHA3-224\t(stream):\t")
		SHA_stream_test(sha3_256,														tests["SHA3_256_stream_tests"],			"SHA3-256\t(stream):\t")
		SHA_stream_test(sha3_384,														tests["SHA3_384_stream_tests"],			"SHA3-384\t(stream):\t")
		SHA_stream_test(sha3_512,														tests["SHA3_512_stream_tests"],			"SHA3-512\t(stream):\t")
	if not no_encryption:
		print(chs.apply("===================[ENCRYPTION]===================", chs.CYAN))
		AES_test(aes.encrypt_ECB, aes.decrypt_ECB,										tests["AES_ECB_tests"],					"AES_ECB:\t\t\t")
		AES_test(aes.encrypt_CBC, aes.decrypt_CBC,										tests["AES_CBC_tests"],					"AES_CBC:\t\t\t")
		AES_test(aes.encrypt_CFB, aes.decrypt_CFB,										tests["AES_CFB_tests"],					"AES_CFB:\t\t\t")
	if not no_crc:
		print(chs.apply("======================[CRC]=======================", chs.CYAN))
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_ccit),			tests["CRC8_CCITT_lookup_table"],		"CRC8_CCITT\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_rohc),			tests["CRC8_ROHC_lookup_table"],		"CRC8_ROHC\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_cdma2000),		tests["CRC8_CDMA2000_lookup_table"],	"CRC8_CDMA2000\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_wcdma),			tests["CRC8_WCDMA_lookup_table"],		"CRC8_WCDMA\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_ebu),			tests["CRC8_EBU_lookup_table"],			"CRC8_EBU\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_i_code),			tests["CRC8_I_CODE_lookup_table"],		"CRC8_I_CODE\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_darc),			tests["CRC8_DARC_lookup_table"],		"CRC8_DARC\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_dvb_s2),			tests["CRC8_DVB_S2_lookup_table"],		"CRC8_DVB_S2\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_itu),			tests["CRC8_ITU_lookup_table"],			"CRC8_ITU\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc8_table, crc_t.crc8_maxim),			tests["CRC8_MAXIM_lookup_table"],		"CRC8_MAXIM\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_ccitt),		tests["CRC16_CCITT_lookup_table"],		"CRC16_CCITT\t(lookup):\t")		# ccitt, aug ccitt, genibus, xmodem are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_aug_ccitt),	tests["CRC16_CCITT_lookup_table"],		"CRC16_AUG_CCITT\t(lookup):\t")	# ccitt, aug ccitt, genibus, xmodem are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_genibus),		tests["CRC16_CCITT_lookup_table"],		"CRC16_GENIBUS\t(lookup):\t")	# ccitt, aug ccitt, genibus, xmodem are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_xmodem),		tests["CRC16_CCITT_lookup_table"],		"CRC16_XMODEM\t(lookup):\t")	# ccitt, aug ccitt, genibus, xmodem are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_mcrf4xx),		tests["CRC16_MCRF4XX_lookup_table"],	"CRC16_MCRF4XX\t(lookup):\t")	# mcrf4xx, riello, tms37157, a, kermit, x25 are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_riello),		tests["CRC16_MCRF4XX_lookup_table"],	"CRC16_RIELLO\t(lookup):\t")	# mcrf4xx, riello, tms37157, a, kermit, x25 are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_tms37157),		tests["CRC16_MCRF4XX_lookup_table"],	"CRC16_TMS37157\t(lookup):\t")	# mcrf4xx, riello, tms37157, a, kermit, x25 are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_a),			tests["CRC16_MCRF4XX_lookup_table"],	"CRC16_A\t\t(lookup):\t")		# mcrf4xx, riello, tms37157, a, kermit, x25 are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_kermit),		tests["CRC16_MCRF4XX_lookup_table"],	"CRC16_KERMIT\t(lookup):\t")	# mcrf4xx, riello, tms37157, a, kermit, x25 are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_x25),			tests["CRC16_MCRF4XX_lookup_table"],	"CRC16_X25\t(lookup):\t")		# mcrf4xx, riello, tms37157, a, kermit, x25 are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_buypass),		tests["CRC16_BUYPASS_lookup_table"],	"CRC16_BUYPASS\t(lookup):\t")	# dds_110, buypass are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_dds_110),		tests["CRC16_BUYPASS_lookup_table"],	"CRC16_DDS_110\t(lookup):\t")	# dds_110, buypass are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_arc),			tests["CRC16_ARC_lookup_table"],		"CRC16_ARC\t(lookup):\t")		# arc, maxim, usb, modbus are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_maxim),		tests["CRC16_ARC_lookup_table"],		"CRC16_MAXIM\t(lookup):\t")		# arc, maxim, usb, modbus are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_usb),			tests["CRC16_ARC_lookup_table"],		"CRC16_USB\t(lookup):\t")		# arc, maxim, usb, modbus are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_modbus),		tests["CRC16_ARC_lookup_table"],		"CRC16_MODBUS\t(lookup):\t")	# arc, maxim, usb, modbus are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_dect_r),		tests["CRC16_DECT_R_lookup_table"],		"CRC16_DECT_R\t(lookup):\t")	# dect_r, dect_x are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_dect_x),		tests["CRC16_DECT_R_lookup_table"],		"CRC16_DECT_X\t(lookup):\t")	# dect_r, dect_x are the same
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_en_13757),		tests["CRC16_EN_13757_lookup_table"],	"CRC16_EN_13757\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_dnp),			tests["CRC16_DNP_lookup_table"],		"CRC16_DNP\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_cdma2000),		tests["CRC16_CDMA2000_lookup_table"],	"CRC16_CDMA2000\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc16_table, crc_t.crc16_teledisk),		tests["CRC16_TELEDISK_lookup_table"],	"CRC16_TELEDISK\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_bzip2),		tests["CRC32_MPEG2_lookup_table"],		"CRC32_BZIP2\t(lookup):\t")  	# bzip2, mpeg, posix are the same
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_mpeg2),		tests["CRC32_MPEG2_lookup_table"],		"CRC32_MPEG2\t(lookup):\t")		# bzip2, mpeg, posix are the same
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_posix),		tests["CRC32_MPEG2_lookup_table"],		"CRC32_POSIX\t(lookup):\t")		# bzip2, mpeg, posix are the same
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32),				tests["CRC32_lookup_table"],			"CRC32\t\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_jamcrc),		tests["CRC32_JAMCRC_lookup_table"],		"CRC32_JAMCRC\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_q),			tests["CRC32_Q_lookup_table"],			"CRC32_Q\t\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_xfr),			tests["CRC32_XFR_lookup_table"],		"CRC32_XFR\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_c),			tests["CRC32_C_lookup_table"],			"CRC32_C\t\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc32_table, crc_t.crc32_d),			tests["CRC32_D_lookup_table"],			"CRC32_D\t\t(lookup):\t")
		CRC_tablegen_test(functools.partial(get_crc64_table, crc_t.crc64_ecma),			tests["CRC64_ECMA_lookup_table"],		"CRC64_ECMA\t(lookup):\t")
	if not no_console:
		print(chs.apply("====================[CONSOLE]=====================", chs.CYAN))
		getpass_test()

	print("\n--------------------------------------------------", end="\n\n")


# TODO:
# CRC64, CRC(n) check

# youtube.com/watch?v=_5T70cAXDJ0  # pybind11
# https://github.com/ETLCPP/crc-table-generator
# https://www.youtube.com/watch?v=cvmQNTVJrzs