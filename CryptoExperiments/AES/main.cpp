#include <cstdio>
#include <cstdlib>
#include "TestData.h"
#include "AesTables.h"

struct AESparam
{
  unsigned int Nk, Nb, Nr;
};

AESparam AES128param = { 4, 4, 10 };
AESparam AES192param = { 6, 4, 12 };
AESparam AES256param = { 8, 4, 14 };

unsigned int SubWord(unsigned int value)
{
  return sboxTable[value >> 24 & 0x000000FFU] << 24 & 0xFF000000U
    | sboxTable[value >> 16 & 0x000000FFU] << 16 & 0x00FF0000U
    | sboxTable[value >> 8 & 0x000000FFU] << 8 & 0x0000FF00U
    | sboxTable[value & 0x000000FFU] & 0x000000FFU;
}

unsigned int RotWord(unsigned value)
{
  return value << 8 | value >> 24;
}

void KeyExpansion(unsigned char *key, unsigned int *w, AESparam *aesParam)
{
  for(unsigned int i = 0; i < aesParam->Nk; i++)
  {
    w[i] = key[i * 4] << 24 & 0xFF000000U
      | key[i * 4 + 1] << 16 & 0x00FF0000U
      | key[i * 4 + 2] << 8 & 0x0000FF00U
      | key[i * 4 + 3] & 0x000000FFU;
  }

  for(unsigned int i = aesParam->Nk; i < (aesParam->Nr + 1) * aesParam->Nb; i++)
  {
    unsigned int tmp = w[i - 1];
    if(i % aesParam->Nk == 0)
    {
      tmp = SubWord(RotWord(tmp)) ^ Rcon[i / aesParam->Nk];
    }
    else if(aesParam->Nk > 6 && i % aesParam->Nk == 4)
    {
      tmp = SubWord(tmp);
    }
    w[i] = w[i - aesParam->Nk] ^ tmp;
  }
}

void AddRoundKey(unsigned int *state, unsigned int *w)
{
  state[0] ^= w[0];
  state[1] ^= w[1];
  state[2] ^= w[2];
  state[3] ^= w[3];
}

void Cipher(unsigned char *in, unsigned char *out, unsigned int *w, AESparam *aesParam)
{
  unsigned int state[4];

  for(size_t i = 0; i < aesParam->Nb; i++)
  {
    state[i] = in[i * 4] << 24 & 0xFF000000U
      | in[i * 4 + 1] << 16 & 0x00FF0000U
      | in[i * 4 + 2] << 8 & 0x0000FF00U
      | in[i * 4 + 3] & 0x000000FFU;
  }

  AddRoundKey(state, w);

  for(size_t i = 0; i < aesParam->Nb; i++)
  {
    out[i * 4] = state[i] >> 24 & 0x000000FFU;
    out[i * 4 + 1] = state[i] >> 16 & 0x000000FFU;
    out[i * 4 + 2] = state[i] >> 8 & 0x000000FFU;
    out[i * 4 + 3] = state[i] & 0x000000FFU;
  }
}

int main()
{
  AESparam *selectedAesParam = &AES128param;
  const size_t wSize = (selectedAesParam->Nr + 1) * selectedAesParam->Nb;
  unsigned int *w = new unsigned int[wSize];
  KeyExpansion(cipherKey128, w, selectedAesParam);

  for(size_t i = 0; i < wSize; i++)
  {
    printf("w[%d] = 0x%08x\n", i, w[i]);
  }

  unsigned char cipherOutput[16];

  Cipher(cipherInput, cipherOutput, w ,selectedAesParam);

  for(size_t i = 0; i < 4 * selectedAesParam->Nb; i++)
  {
    printf("%02x", cipherOutput[i]);
  }
  printf("\n");

  return 0;
}