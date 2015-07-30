/*
 * test.c
 *
 *  Created on: 2015. 6. 4.
 *      Author: woong
 */

#include <stdio.h>

#include "lsh_print.h"
#include "benchmark_arm.h"

const static int MAX_COUNT = 10;

uint64_t lsh256_benchmark(const size_t databitlen) {

	ALIGNED(16) uint8_t hashval[32];
	ALIGNED(16) uint8_t data[4096 * 1024];
	uint64_t t0, t1, cal, tmin = 0xffffffffffffffff;
	int i;

	cal = calibrate();

	for (i = 0; i < MAX_COUNT; ++i) {
		t0 = hi_res_time_microsec();

		hash256(256, data, databitlen, hashval);

		t1 = hi_res_time_microsec();

		if (tmin > t1 - t0 - cal) {
			tmin = t1 - t0 - cal;
		}
	}

	return tmin;
}

uint64_t lsh512_benchmark(const size_t databitlen) {
	ALIGNED(16) uint8_t hashval[64];
	ALIGNED(16) uint8_t data[4096 * 1024];
	uint64_t t0, t1, cal, tmin = 0xffffffffffffffff;
	int i;

	cal = calibrate();

	for (i = 0; i < MAX_COUNT; ++i) {
		t0 = hi_res_time_microsec();

		hash512(512, data, databitlen, hashval);

		t1 = hi_res_time_microsec();

		if (tmin > t1 - t0 - cal) {
			tmin = t1 - t0 - cal;
		}
	}

	return tmin;
}

void print_lsh256_benchmark(const size_t databitlen) {
	uint64_t tmin;
	tmin = lsh256_benchmark(databitlen);
	printf("LSH-256-256, %4dK-byte messages: ", databitlen / 1024 / 8);
	printf("%7.2f cycles/byte\n", get_cpb(tmin, databitlen / 8));
}

void print_lsh256_benchmark_long(const size_t databitlen1, const size_t databitlen2) {
	uint64_t tmin;
	tmin = lsh256_benchmark(databitlen1) - lsh256_benchmark(databitlen2);
	printf("LSH-256-256,       long messages: ");
	printf("%7.2f cycles/byte\n", get_cpb(tmin, (databitlen1 - databitlen2) / 8));
}

void print_lsh512_benchmark(const size_t databitlen) {
	uint64_t tmin;
	tmin = lsh512_benchmark(databitlen);
	printf("LSH-512-512, %4dK-byte messages: ", databitlen / 1024 / 8);
	printf("%7.2f cycles/byte\n", get_cpb(tmin, databitlen / 8));
}

void print_lsh512_benchmark_long(const size_t databitlen1, const size_t databitlen2) {
	uint64_t tmin;
	tmin = lsh512_benchmark(databitlen1) - lsh512_benchmark(databitlen2);
	printf("LSH-512-512,       long messages: ");
	printf("%7.2f cycles/byte\n", get_cpb(tmin, (databitlen1 - databitlen2) / 8));
}

int main(void) {
	printf("\nLSH Benchmark Results: NEON\n");
	printf("CPU clock: %lld MHz\n\n", read_cpu_clock_MHz());

	// for 64-byte messages
	print_lsh256_benchmark(0x200 * 1024); // 64 byte
	print_lsh256_benchmark(0x8000 * 1024); // 4096 byte
	print_lsh256_benchmark_long(0x8000 * 1024, 0x4000 * 1024); // long message

	print_lsh512_benchmark(0x200 * 1024); // 64 byte
	print_lsh512_benchmark(0x8000 * 1024); // 4096 byte
	print_lsh512_benchmark_long(0x8000 * 1024, 0x4000 * 1024); // long message

	return 0;
}
