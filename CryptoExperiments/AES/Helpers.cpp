#include "Helpers.h"

unsigned int ConstructUint(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
  return b3 << 24 & 0xFF000000U
    | b2 << 16 & 0x00FF0000U
    | b1 << 8 & 0x0000FF00U
    | b0 & 0x000000FFU;
}