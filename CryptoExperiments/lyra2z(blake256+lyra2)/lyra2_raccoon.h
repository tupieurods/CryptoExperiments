#pragma once

/**
  * Special implementation of lyra2 for lyra2z with params:
  * key length: 32 bytes
  * password length: 32 bytes
  * salt length: 32 bytes (password value == salt value)
  * timeCost: 8
  * nRows: 8
  * nCols: 8
  * bitrate: 12 uint64(96 bytes, 768 bits), capacity: 4 uint64(32 bytes, 256bits)
*/
void Lyra2_32_32_8_8_8(unsigned char *state, const unsigned char *input);