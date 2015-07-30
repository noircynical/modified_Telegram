#ifndef HEADER_LSH_PRINT_H_
#define HEADER_LSH_PRINT_H_

#include "lsh_neon.h"

extern void print_byte_block(const char* title, const BYTE* data, const size_t len);
extern void print_register_lsh256(const char* title, const UREG32* var, const size_t len);
extern void print_register_lsh512(const char* title, const UREG64* var, const size_t len);

#endif /* SRC_LSH_PRINT_H_ */
