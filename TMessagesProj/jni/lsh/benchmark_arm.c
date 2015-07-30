#include "benchmark_arm.h"

#include <fcntl.h>
#include <string.h>
#include <time.h>

const static char* REF_PATH1 = "/sys/devices/system/cpu/cpu0/tsc_freq_khz";
const static char* REF_PATH2 = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq";

static inline int read_long_from_file(const char* filepath, uint64_t* value) {

	int ret = 0;
	int fd = open(filepath, O_RDONLY);
	uint64_t tmp;

	if (fd != -1) {
		char line[1024];
		char* err;
		memset(line, 0, sizeof(line));
		read(fd, line, sizeof(line) - 1);
		tmp = strtol(line, &err, 10);
		if (line[0] != '\0' && (*err == '\n' || *err == '\0')) {
			*value = tmp;
			ret = 1;
		}

		close(fd);
	}

	return ret;
}

uint64_t hi_res_time_microsec() {
	struct timespec tp;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
	return tp.tv_sec * 1000000 + tp.tv_nsec / 1000;
}

uint64_t calibrate() {
	uint64_t t0, t1, cal, tmin = 0xffffffffffffffff;
	int i;

	for (i = 0; i < 1000; ++i) {
		t0 = hi_res_time_microsec();
		t1 = hi_res_time_microsec();

		if (tmin > t1 - t0) {
			tmin = t1 - t0;
		}
	}

	return tmin;
}

uint64_t read_cpu_clock_MHz() {
	uint64_t clock;
	int ret = 0;

	ret = read_long_from_file(REF_PATH1, &clock);
	if (ret) {
		return clock / 1000;
	}

	ret = read_long_from_file(REF_PATH2, &clock);
	if (ret) {
		return clock / 1000;
	}

	// failed to read cpu clock
	return 1;
}

double get_cpb(const uint64_t elapsed_microsec, const size_t datalen_byte) {
#ifdef ENABLE_DEBUG
	printf("microsec = %6" PRIu64 " us\t", microsec);
#endif
	return (double) elapsed_microsec / datalen_byte * read_cpu_clock_MHz();
}
