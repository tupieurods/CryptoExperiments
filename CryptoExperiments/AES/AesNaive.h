#ifndef AESNAIVE_H
#define AESNAIVE_H

#include "AesCommon.h"

void AddRoundKey(unsigned int *state, unsigned int *w);
void ShiftRows(unsigned int *state);
unsigned int MixColumn(unsigned int value);
void MixColumns(unsigned int *state);
void KeyExpansion(unsigned char *key, unsigned int *w, const AESparam *aesParam);
void Cipher(unsigned char *in, unsigned char *out, unsigned int *w, const AESparam *aesParam);

#endif // AESNAIVE_H
