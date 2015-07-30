#include "lsh512_neon.h"
#ifdef ENABLE_DEBUG
#include "lsh_print.h"
#endif

#include <stdio.h>
#include <string.h>

typedef WORD64 WORD;
typedef UREG64 UREG;
typedef SREG64 SREG;
typedef HREG64 HREG;

#define SUBTRACT(r_x,r_y) vsubq_u64(r_x,r_y)
#define LOAD(x) vld1q_u64(x)
#define SET(x) vdupq_n_u64(x)
#define XOR(x, y) veorq_u64(x, y)
#define STORE(x, y) vst1q_u64(x, y)
#define ROTL(r_x, i) vsriq_n_u64(vshlq_n_u64(r_x,i), r_x, 64-i)
#define SHIFTL(r_x,r_info) vshlq_u64(r_x,r_info)

//IV for LSH-512-512
static const WORD CONST_IV512[8][2] = {
		0xadd50f3c7f07094e, 0xe3f3cee8f9418a4f, 0xb527ecde5b3d0ae9, 0x2ef6dec68076f501,
		0x8cb994cae5aca216, 0xfbb9eae4bba48cc7, 0x650a526174725fea, 0x1f9a61a73f8d8085,
		0xb6607378173b539b, 0x1bc99853b0c0b9ed, 0xdf727fc19b182d47, 0xdbef360cf893a457,
		0x4981f5e570147e80, 0xd00c4490ca7d3e30, 0x5d73940c0e4ae1ec, 0x894085e2edb2d819,
};

static const WORD CONST_STEP512[28][4][2] = {
		0x97884283c938982a, 0xba1fca93533e2355, 0xc519a2e87aeb1c03, 0x9a0fc95462af17b1,
		0xfc3dda8ab019a82b, 0x02825d079a895407, 0x79f2d0a7ee06a6f7, 0xd76d15eed9fdf5fe,
		0x1fcac64d01d0c2c1, 0xd9ea5de69161790f, 0xdebc8b6366071fc8, 0xa9d91db711c6c94b,
		0x3a18653ac9c1d427, 0x84df64a223dd5b09, 0x6cc37895f4ad9e70, 0x448304c8d7f3f4d5,
		0xea91134ed29383e0, 0xc4484477f2da88e8, 0x9b47eec96d26e8a6, 0x82f6d4c8d89014f4,
		0x527da0048b95fb61, 0x644406c60138648d, 0x303c0e8aa24c0edc, 0xc787cda0cbe8ca19,
		0x7ba46221661764ca, 0x0c8cbc6acd6371ac, 0xe336b836940f8f41, 0x79cb9da168a50976,
		0xd01da49021915cb3, 0xa84accc7399cf1f1, 0x6c4a992cee5aeb0c, 0x4f556e6cb4b2e3e0,
		0x200683877d7c2f45, 0x9949273830d51db8, 0x19eeeecaa39ed124, 0x45693f0a0dae7fef,
		0xedc234b1b2ee1083, 0xf3179400d68ee399, 0xb6e3c61b4945f778, 0xa4c3db216796c42f,
		0x268a0b04f9ab7465, 0xe2705f6905f2d651, 0x08ddb96e426ff53d, 0xaea84917bc2e6f34,
		0xaff6e664a0fe9470, 0x0aab94d765727d8c, 0x9aa9e1648f3d702e, 0x689efc88fe5af3d3,
		0xb0950ffea51fd98b, 0x52cfc86ef8c92833, 0xe69727b0b2653245, 0x56f160d3ea9da3e2,
		0xa6dd4b059f93051f, 0xb6406c3cd7f00996, 0x448b45f3ccad9ec8, 0x079b8587594ec73b,
		0x45a50ea3c4f9653b, 0x22983767c1f15b85, 0x7dbed8631797782b, 0x485234be88418638,
		0x842850a5329824c5, 0xf6aca914c7f9a04c, 0xcfd139c07a4c670c, 0xa3210ce0a8160242,
		0xeab3b268be5ea080, 0xbacf9f29b34ce0a7, 0x3c973b7aaf0fa3a8, 0x9a86f346c9c7be80,
		0xac78f5d7cabcea49, 0xa355bddcc199ed42, 0xa10afa3ac6b373db, 0xc42ded88be1844e5,
		0x9e661b271cff216a, 0x8a6ec8dd002d8861, 0xd3d2b629beb34be4, 0x217a3a1091863f1a,
		0x256ecda287a733f5, 0xf9139a9e5b872fe5, 0xac0535017a274f7c, 0xf21b7646d65d2aa9,
		0x048142441c208c08, 0xf937a5dd2db5e9eb, 0xa688dfe871ff30b7, 0x9bb44aa217c5593b,
		0x943c702a2edb291a, 0x0cae38f9e2b715de, 0xb13a367ba176cc28, 0x0d91bd1d3387d49b,
		0x85c386603cac940c, 0x30dd830ae39fd5e4, 0x2f68c85a712fe85d, 0x4ffeecb9dd1e94d6,
		0xd0ac9a590a0443ae, 0xbae732dc99ccf3ea, 0xeb70b21d1842f4d9, 0x9f4eda50bb5c6fa8,
		0x4949e69ce940a091, 0x0e608dee8375ba14, 0x983122cba118458c, 0x4eeba696fbb36b25,
		0x7d46f3630e47f27e, 0xa21a0f7666c0dea4, 0x5c22cf355b37cec4, 0xee292b0c17cc1847,
		0x9330838629e131da, 0x6eee7c71f92fce22, 0xc953ee6cb95dd224, 0x3a923d92af1e9073,
		0xc43a5671563a70fb, 0xbc2985dd279f8346, 0x7ef2049093069320, 0x17543723e3e46035,
		0xc3b409b00b130c6d, 0x5d6aee6b28fdf090, 0x1d425b26172ff6ed, 0xcccfd041cdaf03ad,
		0xfe90c7c790ab6cbf, 0xe5af6304c722ca02, 0x70f695239999b39e, 0x6b8b5b07c844954c,
		0x77bdb9bb1e1f7a30, 0xc859599426ee80ed, 0x5f9d813d4726e40a, 0x9ca0120f7cb2b179,
		0x8f588f583c182cbd, 0x951267cbe9eccce7, 0x678bb8bd334d520e, 0xf6e662d00cd9e1b7,
		0x357774d93d99aaa7, 0x21b2edbb156f6eb5, 0xfd1ebe846e0aee69, 0x3cb2218c2f642b15,
		0xe7e7e7945444ea4c, 0xa77a33b5d6b9b47c, 0xf34475f0809f6075, 0xdd4932dce6bb99ad,
		0xacec4e16d74451dc, 0xd4a0a8d084de23d6, 0x1bdd42f278f95866, 0xeed3adbb938f4051,
		0xcfcf7be8992f3733, 0x21ade98c906e3123, 0x37ba66711fffd668, 0x267c0fc3a255478a,
		0x993a64ee1b962e88, 0x754979556301faaa, 0xf920356b7251be81, 0xc281694f22cf923f,
		0x9f4b6481c8666b02, 0xcf97761cfe9f5444, 0xf220d7911fd63e9f, 0xa28bd365f79cd1b0,
		0xd39f5309b1c4b721, 0xbec2ceb864fca51f, 0x1955a0ddc410407a, 0x43eab871f261d201,
		0xeaafe64a2ed16da1, 0x670d931b9df39913, 0x12f868b0f614de91, 0x2e5f395d946e8252,
		0x72f25cbb767bd8f4, 0x8191871d61a1c4dd, 0x6ef67ea1d450ba93, 0x2ea32a645433d344,
		0x9a963079003f0f8b, 0x74a0aeb9918cac7a, 0x0b6119a70af36fa3, 0x8d9896f202f0d480,
		0x654f1831f254cd66, 0x1318a47f0366a25e, 0x65752076250b4e01, 0xd1cd8eb888071772,
		0x30c6a9793f4e9b25, 0x154f684b1e3926ee, 0x6c7ac0b1fe6312ae, 0x262f88f4f3c5550d,
		0xb4674a24472233cb, 0x2bbd23826a090071, 0xda95969b30594f66, 0x9f5c47408f1e8a43,
		0xf77022b88de9c055, 0x64b7b36957601503, 0xe73b72b06175c11a, 0x55b87de8b91a6233,
		0x1bb16e6b6955ff7f, 0xe8e0a5ec7309719c, 0x702c31cb89a8b640, 0xfba387cfada8cde2,
		0x6792db4677aa164c, 0x1c6b1cc0b7751867, 0x22ae2311d736dc01, 0x0e3666a1d37c9588,
		0xcd1fd9d4bf557e9a, 0xc986925f7c7b0e84, 0x9c5dfd55325ef6b0, 0x9f2b577d5676b0dd,
		0xfa6e21be21c062b3, 0x8787dd782c8d7f83, 0xd0d134e90e12dd23, 0x449d087550121d96,
		0xecf9ae9414d41967, 0x5018f1dbf789934d, 0xfa5b52879155a74c, 0xca82d4d3cd278e7c,
		0x688fdfdfe22316ad, 0x0f6555a4ba0d030a, 0xa2061df720f000f3, 0xe1a57dc5622fb3da,
		0xe6a842a8e8ed8153, 0x690acdd3811ce09d, 0x55adda18e6fcf446, 0x4d57a8a0f4b60b46,
		0xf86fbfc20539c415, 0x74bafa5ec7100d19, 0xa824151810f0f495, 0x8723432791e38ebb,
		0x8eeaeb91d66ed539, 0x73d8a1549dfd7e06, 0x0387f2ffe3f13a9b, 0xa5004995aac15193,
		0x682f81c73efdda0d, 0x2fb55925d71d268d, 0xcc392d2901e58a3d, 0xaa666ab975724a42,
};

static const UREG CONST_BP512[4] = { { 0, 16 }, { 32, 48 },
		{ 8, 24 }, { 40, 56 } };

// CONST_BP512[i] - {64, 64}
static const SREG CONST_BPSUB512[4] = {
		0xffffffffffffffc0, 0xffffffffffffffd0, 0xffffffffffffffe0, 0xfffffffffffffff0,
		0xffffffffffffffc8, 0xffffffffffffffd8, 0xffffffffffffffe8, 0xfffffffffffffff8
};

static const int BLK_LEN_BYTE = 256;
static const int ROT_E_a = 23;
static const int ROT_E_b = 59;
static const int ROT_O_a = 7;
static const int ROT_O_b = 3;

#ifdef ENABLE_DEBUG
static int step = 0;

static INLINE void print_step(hashState512* state, int step) {
	printf("\nStep: %d\n", step);
	print_register_lsh512(NULL, state->cv, 8);
}
#endif

static INLINE void load_submsg_even(UREG64* submsg_e, WORD* msgblk) {
	submsg_e[0] = LOAD(msgblk);
	submsg_e[1] = LOAD(msgblk + 2);
	submsg_e[2] = LOAD(msgblk + 4);
	submsg_e[3] = LOAD(msgblk + 6);
	submsg_e[4] = LOAD(msgblk + 8);
	submsg_e[5] = LOAD(msgblk + 10);
	submsg_e[6] = LOAD(msgblk + 12);
	submsg_e[7] = LOAD(msgblk + 14);
}

static INLINE void load_submsg_odd(UREG64* submsg_o, WORD* msgblk) {
	submsg_o[0] = LOAD(msgblk + 16);
	submsg_o[1] = LOAD(msgblk + 18);
	submsg_o[2] = LOAD(msgblk + 20);
	submsg_o[3] = LOAD(msgblk + 22);
	submsg_o[4] = LOAD(msgblk + 24);
	submsg_o[5] = LOAD(msgblk + 26);
	submsg_o[6] = LOAD(msgblk + 28);
	submsg_o[7] = LOAD(msgblk + 30);
}

static INLINE void load_const(UREG64* constR, int r) {
	constR[0] = LOAD(CONST_STEP512[r][0]);
	constR[1] = LOAD(CONST_STEP512[r][1]);
	constR[2] = LOAD(CONST_STEP512[r][2]);
	constR[3] = LOAD(CONST_STEP512[r][3]);
}

static INLINE void add_l(hashState512* state) {
	state->cv[0] += state->cv[4];
	state->cv[1] += state->cv[5];
	state->cv[2] += state->cv[6];
	state->cv[3] += state->cv[7];
}

static INLINE void add_r(hashState512* state) {
	state->cv[4] += state->cv[0];
	state->cv[5] += state->cv[1];
	state->cv[6] += state->cv[2];
	state->cv[7] += state->cv[3];
}

static INLINE void rot_e_a(hashState512* state) {
	state->cv[0] = ROTL(state->cv[0], ROT_E_a);
	state->cv[1] = ROTL(state->cv[1], ROT_E_a);
	state->cv[2] = ROTL(state->cv[2], ROT_E_a);
	state->cv[3] = ROTL(state->cv[3], ROT_E_a);
}

static INLINE void rot_e_b(hashState512* state) {
	state->cv[4] = ROTL(state->cv[4], ROT_E_b);
	state->cv[5] = ROTL(state->cv[5], ROT_E_b);
	state->cv[6] = ROTL(state->cv[6], ROT_E_b);
	state->cv[7] = ROTL(state->cv[7], ROT_E_b);
}

static INLINE void rot_o_a(hashState512* state) {
	state->cv[0] = ROTL(state->cv[0], ROT_O_a);
	state->cv[1] = ROTL(state->cv[1], ROT_O_a);
	state->cv[2] = ROTL(state->cv[2], ROT_O_a);
	state->cv[3] = ROTL(state->cv[3], ROT_O_a);
}

static INLINE void rot_o_b(hashState512* state) {
	state->cv[4] = ROTL(state->cv[4], ROT_O_b);
	state->cv[5] = ROTL(state->cv[5], ROT_O_b);
	state->cv[6] = ROTL(state->cv[6], ROT_O_b);
	state->cv[7] = ROTL(state->cv[7], ROT_O_b);
}

static INLINE void xor_c(hashState512* state, UREG64* constR) {
	state->cv[0] ^= constR[0];
	state->cv[1] ^= constR[1];
	state->cv[2] ^= constR[2];
	state->cv[3] ^= constR[3];
}

static INLINE void byteperm(hashState512* state) {
	state->cv[4] = SHIFTL(state->cv[4],(SREG)(CONST_BP512[0])) ^ SHIFTL(state->cv[4], CONST_BPSUB512[0]);
	state->cv[5] = SHIFTL(state->cv[5],(SREG)(CONST_BP512[1])) ^ SHIFTL(state->cv[5], CONST_BPSUB512[1]);
	state->cv[6] = SHIFTL(state->cv[6],(SREG)(CONST_BP512[2])) ^ SHIFTL(state->cv[6], CONST_BPSUB512[2]);
	state->cv[7] = SHIFTL(state->cv[7],(SREG)(CONST_BP512[3])) ^ SHIFTL(state->cv[7], CONST_BPSUB512[3]);
}

static INLINE void wordperm1(UREG64* x, UREG64* y) {
	HREG* xc = (HREG*) x;
	HREG* yc = (HREG*) y;

	HREG tmp = xc[1];
	*x = vcombine_u64(yc[0], xc[0]);
	*y = vcombine_u64(tmp, yc[1]);
}

static INLINE void wordperm2(UREG64* x, UREG64* y) {
	HREG* xc = (HREG*) x;
	HREG* yc = (HREG*) y;

	HREG tmp = xc[1];
	*x = vcombine_u64(xc[0], yc[1]);
	*y = vcombine_u64(yc[0], tmp);
}

static INLINE void wordperm(hashState512* state) {
	wordperm1(&state->cv[0], &state->cv[1]);
	wordperm1(&state->cv[2], &state->cv[3]);
	wordperm2(&state->cv[4], &state->cv[5]);
	wordperm2(&state->cv[6], &state->cv[7]);

	state->tmp = state->cv[0];
	state->cv[0] = state->cv[2];
	state->cv[2] = state->cv[6];
	state->cv[6] = state->cv[4];
	state->cv[4] = state->tmp;

	state->tmp = state->cv[1];
	state->cv[1] = state->cv[3];
	state->cv[3] = state->cv[7];
	state->cv[7] = state->cv[5];
	state->cv[5] = state->tmp;
}

static INLINE void step_e(hashState512* state, UREG64* constR) {
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

static INLINE void step_o(hashState512* state, UREG64* constR) {
	add_l(state);
	rot_o_a(state);
	xor_c(state, constR);
	add_r(state);
	rot_o_b(state);
	add_l(state);
	byteperm(state);
	wordperm(state);

#ifdef ENABLE_DEBUG
	print_step(state, step++);
#endif
}

static INLINE void msgwordperm1(UREG64* x, UREG64* y) {
	HREG* yc = (HREG*) y;

	UREG64 tmp = *x;
	*x = vcombine_u64(yc[1], yc[0]);
	*y = tmp;
}

static INLINE void msgwordperm2(UREG64* x, UREG64* y) {
	HREG* xc = (HREG*) x;
	HREG* yc = (HREG*) y;

	WORD tmp = xc[1];
	*x = vcombine_u64(yc[1], xc[0]);
	*y = vcombine_u64(tmp, yc[0]);
}

static INLINE void gen_load_submsg(UREG64* submsg1, UREG64* submsg2) {
	msgwordperm1(&submsg1[0], &submsg1[1]);
	msgwordperm2(&submsg1[2], &submsg1[3]);
	msgwordperm1(&submsg1[4], &submsg1[5]);
	msgwordperm2(&submsg1[6], &submsg1[7]);

	submsg1[0] += submsg2[0];
	submsg1[1] += submsg2[1];
	submsg1[2] += submsg2[2];
	submsg1[3] += submsg2[3];
	submsg1[4] += submsg2[4];
	submsg1[5] += submsg2[5];
	submsg1[6] += submsg2[6];
	submsg1[7] += submsg2[7];
}

static INLINE void gen_load_submsg_e(UREG64* submsg_e, UREG64* submsg_o) {
	msgwordperm1(&submsg_e[0], &submsg_e[1]);
	msgwordperm2(&submsg_e[2], &submsg_e[3]);
	msgwordperm1(&submsg_e[4], &submsg_e[5]);
	msgwordperm2(&submsg_e[6], &submsg_e[7]);

	submsg_e[0] += submsg_o[0];
	submsg_e[1] += submsg_o[1];
	submsg_e[2] += submsg_o[2];
	submsg_e[3] += submsg_o[3];
	submsg_e[4] += submsg_o[4];
	submsg_e[5] += submsg_o[5];
	submsg_e[6] += submsg_o[6];
	submsg_e[7] += submsg_o[7];
}

static INLINE void gen_load_submsg_o(UREG64* submsg_e, UREG64* submsg_o) {
	msgwordperm1(&submsg_o[0], &submsg_o[1]);
	msgwordperm2(&submsg_o[2], &submsg_o[3]);
	msgwordperm1(&submsg_o[4], &submsg_o[5]);
	msgwordperm2(&submsg_o[6], &submsg_o[7]);

	submsg_o[0] += submsg_e[0];
	submsg_o[1] += submsg_e[1];
	submsg_o[2] += submsg_e[2];
	submsg_o[3] += submsg_e[3];
	submsg_o[4] += submsg_e[4];
	submsg_o[5] += submsg_e[5];
	submsg_o[6] += submsg_e[6];
	submsg_o[7] += submsg_e[7];
}

static INLINE void xor_with_submsg(hashState512* state, UREG64* submsg) {
	state->cv[0] = XOR(state->cv[0], submsg[0]);
	state->cv[1] = XOR(state->cv[1], submsg[1]);
	state->cv[2] = XOR(state->cv[2], submsg[2]);
	state->cv[3] = XOR(state->cv[3], submsg[3]);
	state->cv[4] = XOR(state->cv[4], submsg[4]);
	state->cv[5] = XOR(state->cv[5], submsg[5]);
	state->cv[6] = XOR(state->cv[6], submsg[6]);
	state->cv[7] = XOR(state->cv[7], submsg[7]);
}

/**
 * @param [in] cv 16 word input
 * @param [in] msgblk 32 word input
 * @param [out] cv 16 word output
 */
static void compress512(hashState512 *state, const BYTE* datablock) {
	int r, step = 0;

	ALIGNED(16) UREG64 constR[4];
	ALIGNED(16) UREG64 submsg_e[8];
	ALIGNED(16) UREG64 submsg_o[8];

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
	for (r = 1; r < 14; ++r) {
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
	print_register_lsh512("Final Step", state->cv, 8);
#endif
}

/**
 * 초기화 작업
 *
 * @param [in] state
 * @param [in] state
 */
int init512(hashState512 *state, const size_t hashbitlen) {
	int i = 0;

	state->hashbitlen = hashbitlen;
	state->remain_msg_bit = 0;
	state->remain_msg_byte = 0;

	for (i = 0; i < 8; ++i) {
		state->cv[i] = LOAD(CONST_IV512[i]);
	}

#ifdef ENABLE_DEBUG
	step = 0;
#endif

	return SUCCESS;
}

int update512(hashState512 *state, const BYTE* data, const size_t databitlen) {

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

		compress512(state, state->data);
		data += more_byte;
		databytelen -= more_byte;
		state->remain_msg_byte = 0;
	}

	while (databytelen >= BLK_LEN_BYTE) {
		compress512(state, data);
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
int final512(hashState512* state, BYTE* hashval) {

	if (state->remain_msg_bit) {
		state->data[state->remain_msg_byte] |= (0x1 << (7 - state->remain_msg_bit));
	} else {
		state->data[state->remain_msg_byte] = 0x80;
	}

	int pos = state->remain_msg_byte + 1;
	memset(state->data + pos, 0x00, BLK_LEN_BYTE - pos);
	compress512(state, state->data);

	state->cv[0] = XOR(state->cv[0], state->cv[4]);
	STORE((WORD* ) hashval, state->cv[0]);

	state->cv[1] = XOR(state->cv[1], state->cv[5]);
	STORE((WORD* ) hashval + 2, state->cv[1]);

	state->cv[2] = XOR(state->cv[2], state->cv[6]);
	STORE((WORD* ) hashval + 4, state->cv[2]);

	state->cv[3] = XOR(state->cv[3], state->cv[7]);
	STORE((WORD* ) hashval + 6, state->cv[3]);

	//memcpy(hashval, state->cv, state->hashbitlen / 8);
	return SUCCESS;
}

/**
 * lsh512 계산
 *
 * @param [in] hashbitlen 출력 해시 비트 길이
 * @param [in] data 입력 데이터
 * @param [in] databitlen 입력 데이터 비트 길이
 *
 * @param [out] hashval 출력 해시
 */
int hash512(const size_t hashbitlen, const BYTE* data, const size_t databitlen, BYTE* hashval) {

	int result, i;
	hashState512 state;

	result = init512(&state, hashbitlen);
	if (result != SUCCESS) {
		return result;
	}

	result = update512(&state, data, databitlen);
	if (result != SUCCESS) {
		return result;
	}

	return final512(&state, hashval);
}
