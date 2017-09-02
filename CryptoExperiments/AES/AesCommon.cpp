#include "Helpers.h"
#include "AesCommon.h"

unsigned int SubWord(unsigned int value)
{
  return ConstructUint(SboxTable[BYTE3(value)], SboxTable[BYTE2(value)], SboxTable[BYTE1(value)], SboxTable[BYTE0(value)]);
}

unsigned int RotWord(unsigned value)
{
  return value << 8 | value >> 24;
}