
#ifndef HEADER_BENCHMARK_ARM_H_
#define HEADER_BENCHMARK_ARM_H_

#include <stdint.h>
#include <stddef.h>

extern uint64_t hi_res_time_microsec();
extern uint64_t calibrate();
extern uint64_t read_cpu_clock_MHz();
extern double get_cpb(const uint64_t elapsed_microsec, const size_t datalen_byte);

#endif /* SRC_BENCHMARK_ARM_H_ */
