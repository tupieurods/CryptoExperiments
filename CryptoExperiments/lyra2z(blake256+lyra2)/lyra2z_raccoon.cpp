#include <cstring>
#include "sph_blake.h"
#include "lyra2z_raccoon.h"
#include "lyra2_raccoon.h"


void Lyra2ZhashRaccoon(unsigned char * state, const unsigned char * input)
{
  sph_blake256_context ctx_blake;

  unsigned int hashA[8], hashB[8];

  sph_blake256_init(&ctx_blake);
  sph_blake256(&ctx_blake, input, 80, 14);
  sph_blake256_close(&ctx_blake, hashA, 14);

  Lyra2_32_32_8_8_8(reinterpret_cast<unsigned char*>(hashB), reinterpret_cast<unsigned char*>(hashA));

  memcpy(state, hashB, 32);
}
