from lib import *


if __name__ == "__main__":
	data = get_crc16_table(crc_t.crc16_dnp)

	print("uint16 crc16_dnp_table[256] = {")
	for i in range(256):
		end = ", "
		if i == 0: print("\t", end="")
		elif i == 255: end = "\n"
		elif (i + 1) % 8 == 0: end = ",\n\t"
		print("0x" + data[2 * i: (2 * i) + 2].hex(), end=end)
	print("};")