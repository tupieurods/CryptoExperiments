#include "lyra2_raccoon.h"
#include "lyra2.h"

void Lyra2_32_32_8_8_8(unsigned char * state, const unsigned char * input)
{
  LYRA2(state, 32, input, 32, input, 32, 8, 8, 8);
}