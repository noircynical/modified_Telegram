#ifndef HEADER_LSH_NEON_H_
#define HEADER_LSH_NEON_H_

#include <arm_neon.h>
#include <stddef.h>
#include <stdint.h>

//#define ENABLE_DEBUG
#define ALIGNED(x) __attribute__((aligned(x)))
//#define INLINE __attribute__((always_inline))
#define INLINE __inline

#define SUCCESS 0
#define FAIL 1

typedef uint8_t BYTE;

// unsigned registers
typedef uint32x4_t UREG32;
typedef uint64x2_t UREG64;

// signed registers
typedef int32x4_t SREG32;
typedef int64x2_t SREG64;

// half-sized registers
typedef uint32x2_t HREG32;
typedef uint64x1_t HREG64;

// word
typedef uint32_t WORD32;
typedef uint64_t WORD64;

#endif /* SRC_LSH_NEON_H_ */
