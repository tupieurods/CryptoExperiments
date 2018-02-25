#include <cstring>
#include <cstdio>
#include "lyra2_raccoon.h"
#include "sponge_raccoon.h"
//#include "lyra2.h"

void Lyra2_32_32_8_8_8(unsigned char *output, const unsigned char *input)
{
  // BEGIN Bootstrapping phase: init sponge state
  uint64_t spongeState[16];
  initState_raccoon(spongeState);
  {
    unsigned char paddedInput[64];
    memcpy(paddedInput, input, 32 * sizeof(unsigned char));
    memcpy(paddedInput + 32, input, 32 * sizeof(unsigned char));
    absorbBlockBlake2Safe_raccoon(spongeState, reinterpret_cast<uint64_t *>(paddedInput));
    memset(paddedInput, 0, 64 * sizeof(unsigned char));
    paddedInput[0] = 32;
    paddedInput[8] = 32;
    paddedInput[16] = 32;
    paddedInput[24] = 8;
    paddedInput[32] = 8;
    paddedInput[40] = 8;
    paddedInput[48] = 0x80;
    paddedInput[63] = 0x01;
    absorbBlockBlake2Safe_raccoon(spongeState, reinterpret_cast<uint64_t *>(paddedInput));
  }
  printf("Lyra2_32_32_8_8_8 state:\n");
  for(int i = 0; i < 16; i++)
  {
    printf("0x%016llx\n", spongeState[i]);
  }
  // END Bootstrapping phase: init sponge state
  //LYRA2(state, 32, input, 32, input, 32, 8, 8, 8);
}
