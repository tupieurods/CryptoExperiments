#include <cstring>

#include "sph_blake.h"
#include "lyra2.h"

void lyra2Zhash(void *state, const void *input)
{
  sph_blake256_context ctx_blake;

  unsigned int hashA[8], hashB[8];

  sph_blake256_init(&ctx_blake);
  sph_blake256(&ctx_blake, input, 80, 14);
  sph_blake256_close(&ctx_blake, hashA, 14);

  LYRA2(hashB, 32, hashA, 32, hashA, 32, 8, 8, 8);

  memcpy(state, hashB, 32);
}
