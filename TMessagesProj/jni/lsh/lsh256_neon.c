#include "lsh256_neon.h"
#ifdef ENABLE_DEBUG
#include "lsh_print.h"
#endif

#include <stdio.h>
#include <string.h>

typedef WORD32 WORD;
typedef UREG32 UREG;
typedef SREG32 SREG;
typedef HREG32 HREG;

#define SUBTRACT(r_x,r_y) vsubq_u32(r_x,r_y)
#define LOAD(x) vld1q_u32(x)
#define SET(x) vdupq_n_u32(x)
#define XOR(x, y) veorq_u32(x, y)
#define STORE(x, y) vst1q_u32(x, y)
#define ROTL(r_x, i) vsriq_n_u32(vshlq_n_u32(r_x,i), r_x, 32-i)
#define SHIFTL(r_x,r_info) vshlq_u32(r_x,r_info)

static const ALIGNED(16) WORD CONST_IV256[4][4] = {
		0x46a10f1f, 0xfddce486, 0xb41443a8, 0x198e6b9d,
		0x3304388d, 0xb0f5a3c7, 0xb36061c4, 0x7adbd553,
		0x105d5378, 0x2f74de54, 0x5c2f2d95, 0xf2553fbe,
		0x8051357a, 0x138668c8, 0x47aa4484, 0xe01afb41
};

static const ALIGNED(16) WORD CONST_STEP256[26][2][4] = { 0x917caf90,
		0x6c1b10a2, 0x6f352943, 0xcf778243, 0x2ceb7472, 0x29e96ff2, 0x8a9ba428,
		0x2eeb2642, 0x0e2c4021, 0x872bb30e, 0xa45e6cb2, 0x46f9c612, 0x185fe69e,
		0x1359621b, 0x263fccb2, 0x1a116870, 0x3a6c612f, 0xb2dec195, 0x02cb1f56,
		0x40bfd858, 0x784684b6, 0x6cbb7d2e, 0x660c7ed8, 0x2b79d88a, 0xa6cd9069,
		0x91a05747, 0xcdea7558, 0x00983098, 0xbecb3b2e, 0x2838ab9a, 0x728b573e,
		0xa55262b5, 0x745dfa0f, 0x31f79ed8, 0xb85fce25, 0x98c8c898, 0x8a0669ec,
		0x60e445c2, 0xfde295b0, 0xf7b5185a, 0xd2580983, 0x29967709, 0x182df3dd,
		0x61916130, 0x90705676, 0x452a0822, 0xe07846ad, 0xaccd7351, 0x2a618d55,
		0xc00d8032, 0x4621d0f5, 0xf2f29191, 0x00c6cd06, 0x6f322a67, 0x58bef48d,
		0x7a40c4fd, 0x8beee27f, 0xcd8db2f2, 0x67f2c63b, 0xe5842383, 0xc793d306,
		0xa15c91d6, 0x17b381e5, 0xbb05c277, 0x7ad1620a, 0x5b40a5bf, 0x5ab901a2,
		0x69a7a768, 0x5b66d9cd, 0xfdee6877, 0xcb3566fc, 0xc0c83a32, 0x4c336c84,
		0x9be6651a, 0x13baa3fc, 0x114f0fd1, 0xc240a728, 0xec56e074, 0x009c63c7,
		0x89026cf2, 0x7f9ff0d0, 0x824b7fb5, 0xce5ea00f, 0x605ee0e2, 0x02e7cfea,
		0x43375560, 0x9d002ac7, 0x8b6f5f7b, 0x1f90c14f, 0xcdcb3537, 0x2cfeafdd,
		0xbf3fc342, 0xeab7b9ec, 0x7a8cb5a3, 0x9d2af264, 0xfacedb06, 0xb052106e,
		0x99006d04, 0x2bae8d09, 0xff030601, 0xa271a6d6, 0x0742591d, 0xc81d5701,
		0xc9a9e200, 0x02627f1e, 0x996d719d, 0xda3b9634, 0x02090800, 0x14187d78,
		0x499b7624, 0xe57458c9, 0x738be2c9, 0x64e19d20, 0x06df0f36, 0x15d1cb0e,
		0x0b110802, 0x2c95f58c, 0xe5119a6d, 0x59cd22ae, 0xff6eac3c, 0x467ebd84,
		0xe5ee453c, 0xe79cd923, 0x1c190a0d, 0xc28b81b8, 0xf6ac0852, 0x26efd107,
		0x6e1ae93b, 0xc53c41ca, 0xd4338221, 0x8475fd0a, 0x35231729, 0x4e0d3a7a,
		0xa2b45b48, 0x16c0d82d, 0x890424a9, 0x017e0c8f, 0x07b5a3f5, 0xfa73078e,
		0x583a405e, 0x5b47b4c8, 0x570fa3ea, 0xd7990543, 0x8d28ce32, 0x7f8a9b90,
		0xbd5998fc, 0x6d7a9688, 0x927a9eb6, 0xa2fc7d23, 0x66b38e41, 0x709e491a,
		0xb5f700bf, 0x0a262c0f, 0x16f295b9, 0xe8111ef5, 0x0d195548, 0x9f79a0c5,
		0x1a41cfa7, 0x0ee7638a, 0xacf7c074, 0x30523b19, 0x09884ecf, 0xf93014dd,
		0x266e9d55, 0x191a6664, 0x5c1176c1, 0xf64aed98, 0xa4b83520, 0x828d5449,
		0x91d71dd8, 0x2944f2d6, 0x950bf27b, 0x3380ca7d, 0x6d88381d, 0x4138868e,
		0x5ced55c4, 0x0fe19dcb, 0x68f4f669, 0x6e37c8ff, 0xa0fe6e10, 0xb44b47b0,
		0xf5c0558a, 0x79bf14cf, 0x4a431a20, 0xf17f68da, 0x5deb5fd1, 0xa600c86d,
		0x9f6c7eb0, 0xff92f864, 0xb615e07f, 0x38d3e448, 0x8d5d3a6a, 0x70e843cb,
		0x494b312e, 0xa6c93613, 0x0beb2f4f, 0x928b5d63, 0xcbf66035, 0x0cb82c80,
		0xea97a4f7, 0x592c0f3b, 0x947c5f77, 0x6fff49b9, 0xf71a7e5a, 0x1de8c0f5,
		0xc2569600, 0xc4e4ac8c, 0x823c9ce1 };

static const ALIGNED(16) UREG CONST_BP256[2] = { { 0, 8, 16, 24 }, { 24, 16, 8,	0 } };

// CONST_BP256[i] - {32, 32, 32, 32}
static const ALIGNED(16) SREG CONST_BPSUB256[4] = {
		0xffffffe0, 0xffffffe8, 0xfffffff0, 0xfffffff8,
		0xfffffff8, 0xfffffff0, 0xffffffe8, 0xffffffe0,
};

static const int BLK_LEN_BYTE = 128;
static const int ROT_E_a = 29;
static const int ROT_E_b = 1;
static const int ROT_O_a = 5;
static const int ROT_O_b = 17;

#ifdef ENABLE_DEBUG
static int step = 0;

static INLINE void print_step(hashState256* state, int step) {
	printf("\nafter step: %d\n", step);
	print_register_lsh256(NULL, state->cv, 4);
}
#endif

static INLINE void load_submsg_even(UREG32* submsg_e, uint32_t* msgblk) {
	submsg_e[0] = LOAD(msgblk);
	submsg_e[1] = LOAD(msgblk + 4);
	submsg_e[2] = LOAD(msgblk + 8);
	submsg_e[3] = LOAD(msgblk + 12);
}

static INLINE void load_submsg_odd(UREG32* submsg_o, uint32_t* msgblk) {
	submsg_o[0] = LOAD(msgblk + 16);
	submsg_o[1] = LOAD(msgblk + 20);
	submsg_o[2] = LOAD(msgblk + 24);
	submsg_o[3] = LOAD(msgblk + 28);
}

static INLINE void load_const(UREG32* constR, int r) {
	constR[0] = LOAD(CONST_STEP256[r][0]);
	constR[1] = LOAD(CONST_STEP256[r][1]);
}

static INLINE void add_l(hashState256* state) {
	state->cv[0] += state->cv[2];
	state->cv[1] += state->cv[3];
}

static INLINE void add_r(hashState256* state) {
	state->cv[2] += state->cv[0];
	state->cv[3] += state->cv[1];
}

static INLINE void rot_e_a(hashState256* state) {
	state->cv[0] = ROTL(state->cv[0], ROT_E_a);
	state->cv[1] = ROTL(state->cv[1], ROT_E_a);
}

static INLINE void rot_e_b(hashState256* state) {
	state->cv[2] = ROTL(state->cv[2], ROT_E_b);
	state->cv[3] = ROTL(state->cv[3], ROT_E_b);
}

static INLINE void rot_o_a(hashState256* state) {
	state->cv[0] = ROTL(state->cv[0], ROT_O_a);
	state->cv[1] = ROTL(state->cv[1], ROT_O_a);
}

static INLINE void rot_o_b(hashState256* state) {
	state->cv[2] = ROTL(state->cv[2], ROT_O_b);
	state->cv[3] = ROTL(state->cv[3], ROT_O_b);
}

static INLINE void xor_c(hashState256* state, UREG32* constR) {
	state->cv[0] ^= constR[0];
	state->cv[1] ^= constR[1];
}

static INLINE void byteperm(hashState256* state) {
    /*
	state->cv[2] =
	SHIFTL(state->cv[2],
			(SREG )(CONST_BP256[0])) ^ SHIFTL(state->cv[2], (SREG) SUBTRACT(CONST_BP256[0], WORD_BIT_LEN));

	state->cv[3] =
	SHIFTL(state->cv[3],
			(SREG )(CONST_BP256[1])) ^ SHIFTL(state->cv[3], (SREG) SUBTRACT(CONST_BP256[1], WORD_BIT_LEN));
	*/
	state->cv[2] = SHIFTL(state->cv[2], (SREG )(CONST_BP256[0])) ^ SHIFTL(state->cv[2], CONST_BPSUB256[0]);
	state->cv[3] = SHIFTL(state->cv[3], (SREG )(CONST_BP256[1])) ^ SHIFTL(state->cv[3], CONST_BPSUB256[1]);
	
}

static INLINE void wordperm1(UREG* var) {
	HREG tmp1, tmp2;
	tmp2 = vget_high_u32(*var);
	tmp1 = vget_low_u32(*var);
	tmp2 = vrev64_u32(tmp2);
	*var = vcombine_u32(tmp1, tmp2);
	*var = vextq_u32(*var, *var, 3);
}

static INLINE void wordperm2(UREG* var) {
	*var = vextq_u32(*var, *var, 3);
	*var = vrev64q_u32(*var);
}

static INLINE void wordperm(hashState256* state) {
	wordperm1(&state->cv[0]);
	wordperm1(&state->cv[1]);
	wordperm2(&state->cv[2]);
	wordperm2(&state->cv[3]);

	state->tmp = state->cv[0];
	state->cv[0] = state->cv[1];
	state->cv[1] = state->cv[3];
	state->cv[3] = state->cv[2];
	state->cv[2] = state->tmp;
}

static INLINE void step_e(hashState256* state, UREG32* constR) {
	add_l(state); // add_right_to_left
	rot_e_a(state); // rot_alpha_e
	xor_c(state, constR); // xor_with_const
	add_r(state); // add_left_to_right
	rot_e_b(state); // rot_beta_e
	add_l(state); // add_right_to_left
	byteperm(state); // rot_gamma
	wordperm(state); // word_perm

#ifdef ENABLE_DEBUG
	print_step(state, step++);
#endif
}

static INLINE void step_o(hashState256* state, UREG32* constR) {
	add_l(state);
	rot_o_a(state);
	xor_c(state, constR); // xor_with_const
	add_r(state);
	rot_o_b(state);
	add_l(state);
	byteperm(state);
	wordperm(state);

#ifdef ENABLE_DEBUG
	print_step(state, step++);
#endif
}

static INLINE void msgwordperm1(UREG* var) {
	HREG tmp1, tmp2;
	tmp2 = vget_high_u32(*var);
	tmp1 = vget_low_u32(*var);
	tmp2 = vrev64_u32(tmp2);
	*var = vcombine_u32(tmp1, tmp2);
	*var = vextq_u32(*var, *var, 2);
}

static INLINE void msgwordperm2(UREG* var) {
	*var = vextq_u32(*var, *var, 3);
}

static INLINE void gen_load_submsg(UREG32* submsg1, UREG32* submsg2) {
	msgwordperm1(&submsg1[0]);
	msgwordperm2(&submsg1[1]);
	msgwordperm1(&submsg1[2]);
	msgwordperm2(&submsg1[3]);

	submsg1[0] += submsg2[0];
	submsg1[1] += submsg2[1];
	submsg1[2] += submsg2[2];
	submsg1[3] += submsg2[3];
}

static INLINE void xor_with_submsg(hashState256* state, UREG32* submsg) {
	state->cv[0] = XOR(state->cv[0], submsg[0]);
	state->cv[1] = XOR(state->cv[1], submsg[1]);
	state->cv[2] = XOR(state->cv[2], submsg[2]);
	state->cv[3] = XOR(state->cv[3], submsg[3]);
}

/**
 * @param [in] cv 16 word input
 * @param [in] msgblk 32 word input
 * @param [out] cv 16 word output
 */
static void compress256(hashState256 *state, const BYTE* datablock) {
	int r, step = 0;

	ALIGNED(16) UREG32 constR[2];
	ALIGNED(16) UREG32 submsg_e[4];
	ALIGNED(16) UREG32 submsg_o[4];

#ifdef ENABLE_DEBUG
	print_byte_block("data to compress", datablock, 128);
#endif

	load_submsg_even(submsg_e, (WORD*) datablock);
	xor_with_submsg(state, submsg_e);
	load_const(constR, 0);
	step_e(state, constR);

	load_submsg_odd(submsg_o, (WORD*) datablock);
	xor_with_submsg(state, submsg_o);
	load_const(constR, 1);
	step_o(state, constR);

	// step operation
	for (r = 1; r < 13; ++r) {
		// estep
		gen_load_submsg(submsg_e, submsg_o);
		xor_with_submsg(state, submsg_e);
		load_const(constR, 2 * r);
		step_e(state, constR);

		// ostep
		gen_load_submsg(submsg_o, submsg_e);
		xor_with_submsg(state, submsg_o);
		load_const(constR, 2 * r + 1);
		step_o(state, constR);
	}

	gen_load_submsg(submsg_e, submsg_o);
	xor_with_submsg(state, submsg_e);

#ifdef ENABLE_DEBUG
	printf("\n");
	print_register_lsh256("after compress", state->cv, 4);
#endif
}

/**
 * 초기화 작업
 *
 * @param [in] state
 * @param [in] state
 */
int init256(hashState256 *state, const size_t hashbitlen) {
	int i = 0;

	state->hashbitlen = hashbitlen;
	state->remain_msg_bit = 0;
	state->remain_msg_byte = 0;

	for (i = 0; i < 4; ++i) {
		state->cv[i] = LOAD(CONST_IV256[i]);
	}

#ifdef ENABLE_DEBUG
	step = 0;
#endif

	return SUCCESS;
}

int update256(hashState256 *state, const BYTE* data, const size_t databitlen) {

	size_t databytelen = databitlen >> 3;
	uint32_t bitpos = databitlen & 0x7;

	if (state->remain_msg_bit > 0) {
		state->remain_msg_bit = 0;
		state->remain_msg_byte++;
	}

	if (databytelen + state->remain_msg_byte < BLK_LEN_BYTE) {
		memcpy(state->data + state->remain_msg_byte, data, databytelen);
		state->remain_msg_byte += databytelen;
		state->remain_msg_bit = bitpos;
		if (bitpos) {
			state->data[state->remain_msg_byte] = data[databytelen] & ((0xff >> bitpos) ^ 0xff);
		}

		return SUCCESS;
	}

	if (state->remain_msg_byte > 0) {
		size_t more_byte = BLK_LEN_BYTE - state->remain_msg_byte;
		memcpy(state->data + state->remain_msg_byte, data, more_byte);

		compress256(state, state->data);
		data += more_byte;
		databytelen -= more_byte;
		state->remain_msg_byte = 0;
	}

	while (databytelen >= BLK_LEN_BYTE) {
		compress256(state, data);
		data += BLK_LEN_BYTE;
		databytelen -= BLK_LEN_BYTE;
	}

	if (databytelen > 0) {
		memcpy(state->data, data, databytelen);
		state->remain_msg_byte = databytelen;
	}

	if (bitpos) {
		state->data[databytelen] = data[databytelen] & ((0xff >> bitpos) ^ 0xff);
		state->remain_msg_bit = bitpos;
	}

	return SUCCESS;
}

/**
 * @param [in] state
 * @param [out] hashval 결과값
 */
int final256(hashState256* state, BYTE* hashval) {

	if (state->remain_msg_bit) {
		state->data[state->remain_msg_byte] |= (0x1 << (7 - state->remain_msg_bit));
	} else {
		state->data[state->remain_msg_byte] = 0x80;
	}

	int pos = state->remain_msg_byte + 1;
	memset(state->data + pos, 0x00, BLK_LEN_BYTE - pos);
	compress256(state, state->data);

	state->cv[0] = XOR(state->cv[0], state->cv[2]);
	STORE((WORD* ) hashval, state->cv[0]);

	state->cv[1] = XOR(state->cv[1], state->cv[3]);
	STORE((WORD* ) hashval + 4, state->cv[1]);

	return SUCCESS;
}

/**
 * lsh256 계산
 *
 * @param [in] hashbitlen 출력 해시 비트 길이
 * @param [in] data 입력 데이터
 * @param [in] databitlen 입력 데이터 비트 길이
 *
 * @param [out] hashval 출력 해시
 */
int hash256(const size_t hashbitlen, const BYTE* data, const size_t databitlen, BYTE* hashval) {

	int result, i;
	hashState256 state;

	result = init256(&state, hashbitlen);
	if (result != SUCCESS) {
		return result;
	}

	result = update256(&state, data, databitlen);
	if (result != SUCCESS) {
		return result;
	}

	return final256(&state, hashval);
}

