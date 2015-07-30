#include "lsh_print.h"

#include <stdio.h>

extern void print_byte_block(const char* title, const BYTE* data, const size_t len) {
	int i;

	if (data == NULL) {
		printf("nothing to print\n");
	}

	if (title != NULL) {
		printf("%s\n", title);
	}

	for (i = 0; i < len; ++i) {
		printf("%02x", data[i]);

		if (i > 0) {
			if ((i + 1) < len && (i & 31) == 31) {
				printf("\n");
				continue;
			}

			if ((i & 0x03) == 0x03) {
				printf(" ");
			}
		}
	}
	printf("\n");
}

void print_register_lsh256(const char* title, const UREG32* var, const size_t len) {
	int i, maxidx = len * 4;
	WORD32* ptr = (WORD32*) var;

	if (var == NULL) {
		printf("nothing to print\n");
		return;
	}

	if (title != NULL) {
		printf("%s\n", title);
	}

	for (i = 0; i < maxidx; ++i) {
		printf("%08x ", ptr[i]);

		if (i > 0 && (i + 1) < maxidx && (i & 0x07) == 0x07) {
			printf("\n");
		}
	}
	printf("\n");
}

void print_register_lsh512(const char* title, const UREG64* var, const size_t len) {

	int i;
	uint64_t* ptr = (uint64_t*) var;

	if (title != NULL) {
		printf("%s\n", title);
	}

	for (i = 0; i < len * 2; ++i) {
		printf("%016llx ", ptr[i]);
		if ((i + 1) % 4 == 0) {
			printf("\n");
		}
	}
}
