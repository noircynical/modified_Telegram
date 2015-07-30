#include <stdio.h>
#include "lsh_print.h"
#include "lsh512_neon.h"

BYTE EXP0000[64] = {0x11, 0x8a, 0x2f, 0xf2, 0xa9, 0x9e, 0x3b, 0x21, 0x34, 0x12, 0x5e, 0x2b, 0xaf, 0x20, 0xeb, 0xe3, 0xbd, 0xd0, 0x34, 0xd5, 0xa6, 0x9b, 0x29, 0xc2, 0x2f, 0xc4, 0x99, 0x50, 0x63, 0x34, 0x0b, 0x46, 0x69, 0x78, 0x01, 0xd7, 0xf7, 0xfb, 0x00, 0x70, 0x56, 0x8f, 0x78, 0xe8, 0xed, 0x51, 0x42, 0x15, 0xfc, 0x70, 0xaf, 0x27, 0xd6, 0xf2, 0x7b, 0x01, 0xaa, 0x8a, 0x1d, 0xa7, 0x2b, 0x14, 0xce, 0x7c};

BYTE EXP0001[64] = {0x1e, 0xd1, 0x93, 0x0d, 0x5e, 0xbb, 0xd5, 0x81, 0xe2, 0xe5, 0xe8, 0x13, 0x77, 0x62, 0x04, 0xab, 0xcd, 0xf3, 0x1a, 0x32, 0xec, 0x2b, 0xd8, 0x3d, 0x63, 0x5a, 0x2d, 0x9b, 0x86, 0xa6, 0x81, 0xbf, 0x23, 0xec, 0x76, 0x69, 0x62, 0x05, 0xab, 0x89, 0xeb, 0xa5, 0xab, 0x27, 0x36, 0x81, 0x52, 0x2d, 0xb3, 0xaf, 0xea, 0x71, 0x77, 0x6c, 0xa0, 0xd8, 0x8e, 0x26, 0x9c, 0xcb, 0x03, 0x57, 0x67, 0x90};
BYTE EXP0002[64] = {0x65, 0xd7, 0x4e, 0x79, 0x05, 0xcb, 0xed, 0x1b, 0x1f, 0xcd, 0xaf, 0xae, 0xcb, 0x16, 0x44, 0xc6, 0x43, 0x34, 0x45, 0xea, 0x9a, 0x75, 0x75, 0xa4, 0x4e, 0x42, 0xa9, 0x52, 0x0d, 0x94, 0x15, 0xed, 0xa3, 0x05, 0x87, 0x1a, 0xad, 0x64, 0x51, 0x0a, 0x67, 0x34, 0x90, 0x47, 0x06, 0x95, 0x13, 0xc9, 0x96, 0x95, 0x42, 0x47, 0x53, 0xdd, 0xd8, 0x8a, 0xff, 0xdb, 0x10, 0x8a, 0x79, 0x5f, 0x72, 0x82};
BYTE EXP0007[64] = {0x43, 0xeb, 0xf6, 0xcf, 0xb5, 0xaf, 0xe5, 0xe6, 0xd4, 0xbb, 0x19, 0x00, 0xa4, 0xae, 0xb9, 0x91, 0x80, 0xe4, 0x8c, 0x94, 0x43, 0x88, 0xd9, 0xb8, 0xb8, 0x2e, 0xd1, 0x1e, 0x3f, 0xf2, 0x8b, 0x7a, 0x89, 0x0a, 0xe5, 0x14, 0x8c, 0x88, 0xcc, 0x60, 0x94, 0x82, 0xa5, 0x62, 0x83, 0x4f, 0x0e, 0xd4, 0xde, 0x91, 0x68, 0xf2, 0x1c, 0x86, 0x79, 0x3d, 0xa9, 0x37, 0x37, 0xea, 0x73, 0x04, 0xba, 0x8f};

BYTE EXP0008[64] = {0x48, 0xd2, 0xda, 0xa6, 0x96, 0x72, 0x86, 0x8d, 0x4a, 0x0d, 0x68, 0x2a, 0xb7, 0x4d, 0x3e, 0x78, 0xdb, 0x79, 0x31, 0x39, 0x28, 0x3a, 0xca, 0x40, 0x84, 0x60, 0xfa, 0xc3, 0xaa, 0xe5, 0xd9, 0x58, 0x02, 0x6d, 0x24, 0xae, 0xa8, 0x41, 0xf8, 0x30, 0x2a, 0x89, 0xff, 0xca, 0xd0, 0x4a, 0x03, 0x12, 0xfe, 0xb3, 0x7e, 0x0c, 0x73, 0x4e, 0x7c, 0xbc, 0xe6, 0xb3, 0x61, 0x22, 0x02, 0x85, 0x16, 0x98};
BYTE EXP0015[64] = {0x65, 0xa5, 0x32, 0x6f, 0x88, 0x9a, 0x1d, 0x3f, 0xd0, 0x8c, 0x22, 0x3f, 0x48, 0xdb, 0x1a, 0x0f, 0x3d, 0x66, 0x68, 0x99, 0xe0, 0xb5, 0x01, 0x43, 0xdc, 0xb7, 0x78, 0x71, 0x93, 0x70, 0x24, 0x53, 0x72, 0x3b, 0x3c, 0xa8, 0xd2, 0x67, 0xcb, 0xfb, 0xe3, 0xa3, 0x29, 0x11, 0xef, 0x6a, 0x29, 0xed, 0x20, 0xe5, 0x3c, 0x3e, 0x18, 0xfa, 0x9c, 0x32, 0x32, 0xe5, 0x71, 0xb4, 0xc9, 0x41, 0x53, 0xaa};
BYTE EXP0016[64] = {0x1e, 0x17, 0x58, 0x74, 0x16, 0x43, 0xa7, 0xb0, 0x96, 0x98, 0x6f, 0xa7, 0xec, 0x1b, 0x7a, 0x04, 0xe1, 0x39, 0x7f, 0x86, 0xd9, 0x1d, 0x3b, 0xd9, 0x2f, 0x25, 0x3c, 0x87, 0x5e, 0xda, 0xc7, 0x59, 0xc5, 0xe7, 0xbb, 0x63, 0x28, 0x8f, 0x7f, 0x5e, 0xb2, 0xa0, 0x99, 0x37, 0x60, 0x05, 0x66, 0x72, 0x63, 0x39, 0x31, 0xfa, 0x0e, 0x04, 0x2d, 0xc1, 0xbf, 0x1e, 0x14, 0x45, 0xf9, 0xfb, 0x37, 0xcd};

BYTE EXP2047[64] = {0x89, 0x66, 0x48, 0x50, 0xdb, 0x35, 0x80, 0xf1, 0xe7, 0xf1, 0x13, 0x9f, 0xc8, 0xf1, 0x49, 0x49, 0xf9, 0x60, 0xa9, 0xb6, 0x6a, 0x6e, 0xba, 0x58, 0x85, 0xee, 0xf5, 0x59, 0xe7, 0x0d, 0x6d, 0x89, 0x7c, 0x91, 0x9f, 0x5a, 0x41, 0xef, 0x5c, 0x6a, 0xec, 0x50, 0x23, 0x4f, 0xff, 0x56, 0xb4, 0xb4, 0xf3, 0x38, 0x8e, 0x85, 0x75, 0x62, 0xdb, 0xef, 0x3a, 0x0b, 0x10, 0xbd, 0x7e, 0x22, 0x8f, 0xec};
BYTE EXP2048[64] = {0x9c, 0x2b, 0xb4, 0xe6, 0xb4, 0x5b, 0xa3, 0x35, 0x0e, 0x3a, 0x22, 0x91, 0x05, 0x14, 0xcd, 0xfe, 0xff, 0xa8, 0x21, 0xe1, 0x6f, 0xb3, 0x9d, 0x29, 0xd9, 0x23, 0xc9, 0xe5, 0xfd, 0x98, 0x04, 0x5f, 0x4a, 0x9d, 0xfe, 0x83, 0xec, 0x18, 0x04, 0x8b, 0xde, 0xc1, 0xcd, 0x7f, 0x79, 0x31, 0x93, 0x99, 0xe1, 0x95, 0xb8, 0x5e, 0xd2, 0x17, 0xf2, 0x5e, 0x21, 0xa2, 0x5c, 0x73, 0x77, 0xf3, 0x4c, 0xa0};
BYTE EXP2049[64] = {0xc2, 0x8f, 0x31, 0xe7, 0xdc, 0x19, 0x47, 0xaa, 0xea, 0x0d, 0xb7, 0x26, 0xea, 0xa0, 0x8f, 0x4a, 0xb2, 0x77, 0xa0, 0x03, 0x35, 0xc3, 0xe5, 0x33, 0xd8, 0xf0, 0x33, 0xf6, 0x2e, 0x30, 0x20, 0x9d, 0x7d, 0x17, 0x39, 0x3a, 0xda, 0xed, 0xb2, 0x82, 0x22, 0xc8, 0x6a, 0x1d, 0xe8, 0xb3, 0xfa, 0xab, 0x48, 0x16, 0x70, 0x79, 0xf8, 0x51, 0xb9, 0x30, 0xc1, 0x5d, 0xc6, 0xff, 0x30, 0xbb, 0x65, 0x58};

BYTE EXP4095[64] = {0xf4, 0x42, 0x6a, 0xe3, 0xe2, 0xff, 0xe4, 0x8d, 0x8d, 0xae, 0x3a, 0xaa, 0x17, 0x45, 0xc4, 0x24, 0x83, 0x2a, 0xec, 0x0c, 0x1e, 0x9c, 0x7b, 0xd6, 0xf8, 0x75, 0xa7, 0xcd, 0x78, 0x6d, 0x4b, 0xcf, 0xa3, 0x74, 0xbd, 0xeb, 0x28, 0x85, 0x05, 0x3e, 0xde, 0x76, 0x19, 0x13, 0xe5, 0x70, 0xa1, 0x30, 0x6c, 0xe9, 0x9b, 0xe4, 0xbe, 0x29, 0xf5, 0xe4, 0x4c, 0xdc, 0x49, 0xdb, 0x29, 0x08, 0xcc, 0x8d};
BYTE EXP4096[64] = {0xe2, 0xa4, 0xc9, 0x26, 0xf2, 0x59, 0x27, 0x2c, 0x8c, 0x94, 0x82, 0xcb, 0x57, 0x8b, 0xac, 0xbd, 0x59, 0xcc, 0xee, 0x78, 0x82, 0x27, 0x9e, 0xe7, 0x4d, 0x83, 0x8c, 0x2f, 0x68, 0xa7, 0xfd, 0xd5, 0x47, 0x89, 0x6f, 0x99, 0x3a, 0x52, 0x28, 0x3d, 0xa9, 0x37, 0x59, 0x97, 0xc6, 0x3e, 0xa9, 0xe4, 0x55, 0x8f, 0x9e, 0xfc, 0x0e, 0x86, 0xd0, 0xd6, 0x5f, 0xd4, 0xe3, 0x46, 0xfe, 0x3d, 0x02, 0xbf};
BYTE EXP4097[64] = {0x31, 0x57, 0x6f, 0x84, 0x86, 0x0f, 0x85, 0x60, 0x03, 0x5b, 0xce, 0xe6, 0xf8, 0xd5, 0x87, 0xe3, 0xab, 0xe8, 0x1f, 0x67, 0xdf, 0x11, 0x84, 0xb7, 0x58, 0x0d, 0xaa, 0xb2, 0xf2, 0xee, 0x54, 0xf2, 0x52, 0x1f, 0x0d, 0x5a, 0x7d, 0xef, 0xde, 0x1c, 0x08, 0x25, 0x4d, 0xc7, 0x4e, 0x6d, 0x99, 0x99, 0x8a, 0xd2, 0x33, 0x74, 0xbc, 0x91, 0x1d, 0xa6, 0xa9, 0x7b, 0x56, 0x18, 0x56, 0x94, 0x8c, 0x21};

BYTE EXP6143[64] = {0x67, 0xfd, 0x33, 0xa2, 0x7a, 0x7f, 0x9d, 0x19, 0xe5, 0xc1, 0x0b, 0x62, 0x70, 0x8e, 0xbf, 0x11, 0xb7, 0x09, 0xaa, 0x85, 0xa8, 0x4c, 0xdc, 0xea, 0x2a, 0xdb, 0xd6, 0x4d, 0xa6, 0xb1, 0x15, 0xcb, 0xc1, 0x84, 0x21, 0xd7, 0x94, 0x3f, 0x60, 0x2d, 0xb1, 0xdf, 0x23, 0xea, 0xba, 0xf9, 0x6a, 0xe8, 0x8f, 0xa5, 0x73, 0xdd, 0xa0, 0x42, 0x1a, 0x61, 0xd5, 0xa2, 0x73, 0x84, 0x2a, 0x3f, 0xd9, 0x16};
BYTE EXP6144[64] = {0x42, 0x5d, 0xcd, 0xc2, 0xb7, 0xdd, 0x92, 0xbe, 0x41, 0x05, 0xd7, 0x5f, 0xce, 0x7c, 0x22, 0xae, 0x42, 0x68, 0xd6, 0x0b, 0xb7, 0xcc, 0xcd, 0x28, 0x1b, 0x2c, 0x2e, 0x34, 0x98, 0x48, 0x73, 0x98, 0xb5, 0x3c, 0xae, 0x23, 0xc8, 0xf8, 0xaa, 0xf7, 0x2e, 0x69, 0xf4, 0x90, 0xea, 0x02, 0x23, 0xd3, 0xd2, 0x9d, 0xba, 0x01, 0x1f, 0x8a, 0x32, 0xe3, 0x85, 0x39, 0xa7, 0x61, 0xc8, 0x12, 0x8f, 0x11};
BYTE EXP6145[64] = {0x8d, 0x8a, 0x71, 0x6d, 0x87, 0xbf, 0xd9, 0xaf, 0x51, 0xeb, 0xeb, 0x57, 0xfd, 0xdb, 0x9e, 0xa6, 0xeb, 0x22, 0x78, 0xae, 0x3d, 0x66, 0xf8, 0x61, 0x74, 0xdf, 0x77, 0x88, 0x63, 0xf0, 0xef, 0x1c, 0xb3, 0x27, 0xe2, 0xfa, 0x1e, 0x6e, 0x95, 0x9b, 0x40, 0xda, 0x22, 0xfa, 0xa7, 0x6c, 0xfe, 0x76, 0xdf, 0x18, 0xf3, 0x99, 0x71, 0xf3, 0x8c, 0xf6, 0x75, 0xb1, 0x3c, 0x4c, 0xdb, 0x96, 0x71, 0xbb};

void test512(int bitlen, BYTE* exp) {
	ALIGNED(16) BYTE data[1024] = { '\0', };
	ALIGNED(16) BYTE hashval[64] = { '\0', };
	int i;

	for (i = 0; i < 1024; ++i) {
		data[i] = (i & 0xff);
	}

	i = hash512(512, data, bitlen, hashval);

	if (memcmp(hashval, exp, 64) != 0) {
		printf("FAILED for bitlen=%d\n", bitlen);
		print_byte_block("expected", exp, 64);
		print_byte_block("but calculated", hashval, 64);
		printf("\n\n");
	} else {
		printf("PASSED for bitlen=%d\n", bitlen);
	}
}

int main(void) {
	printf("LSH-512-512 test\n");

	test512(0, EXP0000);

	test512(1, EXP0001);
	test512(2, EXP0002);
	test512(7, EXP0007);

	test512(8, EXP0008);
	test512(15, EXP0015);
	test512(16, EXP0016);

	test512(2047, EXP2047);
	test512(2048, EXP2048);
	test512(2049, EXP2049);

	test512(4095, EXP4095);
	test512(4096, EXP4096);
	test512(4097, EXP4097);

	test512(6143, EXP6143);
	test512(6144, EXP6144);
	test512(6145, EXP6145);

	return 0;
}
