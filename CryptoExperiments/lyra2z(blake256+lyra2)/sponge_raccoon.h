#pragma once
// this file mostly copied from reference sponge.h(cpp) file
// I have done this because it would give me better understanding what is going on

#include <cstdint>

/*Blake2b IV Array*/
static const uint64_t blake2b_IV_raccoon[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

/*Blake2b's rotation*/
static inline uint64_t rotr64_raccoon(const uint64_t w, const unsigned c) {
  return (w >> c) | (w << (64 - c));
}

/*Blake2b's G function*/
#define G_RACCOON(r,i,a,b,c,d) \
  do { \
    a = a + b; \
    d = rotr64_raccoon(d ^ a, 32); \
    c = c + d; \
    b = rotr64_raccoon(b ^ c, 24); \
    a = a + b; \
    d = rotr64_raccoon(d ^ a, 16); \
    c = c + d; \
    b = rotr64_raccoon(b ^ c, 63); \
  } while(0)


/*One Round of the Blake2b's compression function*/
#define ROUND_LYRA_RACCOON(r)  \
    G_RACCOON(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G_RACCOON(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G_RACCOON(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G_RACCOON(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G_RACCOON(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G_RACCOON(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G_RACCOON(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G_RACCOON(r,7,v[ 3],v[ 4],v[ 9],v[14]);

void initState_raccoon(uint64_t state[/*16*/]);
void absorbBlockBlake2Safe_raccoon(uint64_t *state, const uint64_t *in);
void reducedBlake2bLyra_raccoon(uint64_t *v);