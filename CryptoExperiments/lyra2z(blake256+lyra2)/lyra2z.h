#ifndef LYRA2Z_H
#define LYRA2Z_H

//#define LYRA2Z_SCRATCHBUF_SIZE (24576) // matrix size [12][4][4] uint64_t or equivalent
#define LYRA2Z_SCRATCHBUF_SIZE (12*8*8)
// #define LYRA_SCRATCHBUF_SIZE (1536)
#define LYRA_SECBUF_SIZE (4) // (not used)
void lyra2Zhash(void *state, const void *input);

#endif /* LYRA2REV2_H */
