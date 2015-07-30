#ifndef HEADER_LSH512_NEON_H_
#define HEADER_LSH512_NEON_H_

#include "lsh_neon.h"
#include <stdint.h>

typedef ALIGNED(16) struct {
	ALIGNED(4) size_t hashbitlen;
	ALIGNED(4) size_t remain_msg_bit;
	ALIGNED(4) size_t remain_msg_byte;
	ALIGNED(16) UREG64 cv[8];
	ALIGNED(16) BYTE data[256];

	ALIGNED(16) UREG64 tmp;
} hashState512;

/** @brief 해시 상태 초기화 */
extern int init512(hashState512* state, const size_t hashbitlen);

/** @brief 해시 계산 데이터 처리 */
extern int update512(hashState512* state, const BYTE* data, const size_t databitlen);

/** @brief 마지막 데이터 블록 처리 및 최종 결과 계산 */
extern int final512(hashState512* state, BYTE* hashval);

/** @brief init, update, final을 한꺼번에 수행하는 함수 */
extern int hash512(const size_t hashbitlen, const BYTE* data, const size_t databitlen, BYTE* hashval);

#endif /* SRC_LSH512_NEON_H_ */
