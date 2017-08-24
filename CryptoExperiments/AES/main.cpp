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

#define BYTE_N(x, n) ((x >> 8 * n) & 0x000000FFU)
#define BYTE0(x) BYTE_N(x, 0)
#define BYTE1(x) BYTE_N(x, 1)
#define BYTE2(x) BYTE_N(x, 2)
#define BYTE3(x) BYTE_N(x, 3)

// xtime is a macro that finds the product of {02} and the argument to
// xtime modulo {1b}
#define XTIME(x) ((x<<1) ^ (((x>>7) & 1) * 0x1b))
#define X3(x) (x ^ XTIME(x))

unsigned int constructUint(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
  return b3 << 24 & 0xFF000000U
    | b2 << 16 & 0x00FF0000U
    | b1 << 8 & 0x0000FF00U
    | b0 & 0x000000FFU;
}

unsigned int SubWord(unsigned int value)
{
  return constructUint(sboxTable[BYTE3(value)], sboxTable[BYTE2(value)], sboxTable[BYTE1(value)], sboxTable[BYTE0(value)]);
}

unsigned int RotWord(unsigned value)
{
  return value << 8 | value >> 24;
}

void AddRoundKey(unsigned int *state, unsigned int *w)
{
  state[0] ^= w[0];
  state[1] ^= w[1];
  state[2] ^= w[2];
  state[3] ^= w[3];
}

void ShiftRows(unsigned int *state)
{
  unsigned int tmp[4];
  tmp[0] = constructUint(BYTE3(state[0]), BYTE2(state[1]), BYTE1(state[2]), BYTE0(state[3]));
  tmp[1] = constructUint(BYTE3(state[1]), BYTE2(state[2]), BYTE1(state[3]), BYTE0(state[0]));
  tmp[2] = constructUint(BYTE3(state[2]), BYTE2(state[3]), BYTE1(state[0]), BYTE0(state[1]));
  tmp[3] = constructUint(BYTE3(state[3]), BYTE2(state[0]), BYTE1(state[1]), BYTE0(state[2]));
  state[0] = tmp[0];
  state[1] = tmp[1];
  state[2] = tmp[2];
  state[3] = tmp[3];
}

unsigned int MixColumn(unsigned int value)
{
  return constructUint(
    XTIME(BYTE3(value)) ^ X3(BYTE2(value)) ^ BYTE1(value) ^ BYTE0(value),
    BYTE3(value) ^ XTIME(BYTE2(value)) ^ X3(BYTE1(value)) ^ BYTE0(value),
    BYTE3(value) ^ BYTE2(value) ^ XTIME(BYTE1(value)) ^ X3(BYTE0(value)),
    X3(BYTE3(value)) ^ BYTE2(value) ^ BYTE1(value) ^ XTIME(BYTE0(value))
  );
}

void MixColumns(unsigned int *state)
{
  state[0] = MixColumn(state[0]);
  state[1] = MixColumn(state[1]);
  state[2] = MixColumn(state[2]);
  state[3] = MixColumn(state[3]);
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

  for(int i = 1; i < aesParam->Nr; i++)
  {
    //SubBytes
    state[0] = SubWord(state[0]);
    state[1] = SubWord(state[1]);
    state[2] = SubWord(state[2]);
    state[3] = SubWord(state[3]);
    //ShiftRows
    ShiftRows(state);
    //MixColumns
    MixColumns(state);
    //AddRoundKey
    AddRoundKey(state, w + 4 * i);
  }

  //SubBytes
  state[0] = SubWord(state[0]);
  state[1] = SubWord(state[1]);
  state[2] = SubWord(state[2]);
  state[3] = SubWord(state[3]);
  //ShiftRows
  ShiftRows(state);
  //AddRoundKey
  AddRoundKey(state, w + 4 * aesParam->Nr);

  for(size_t i = 0; i < aesParam->Nb; i++)
  {
    out[i * 4] = BYTE3(state[i]);
    out[i * 4 + 1] = BYTE2(state[i]);
    out[i * 4 + 2] = BYTE1(state[i]);
    out[i * 4 + 3] = BYTE0(state[i]);
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

  int failedBytes = 0;
  for(size_t i = 0; i < 4 * selectedAesParam->Nb; i++)
  {
    if(cipherOutput[i] != cipherOutput128[i])
    {
      failedBytes++;
    }
    printf("%02x", cipherOutput[i]);
  }
  printf("\n");
  if(failedBytes)
  {
    printf("Total bytes failed: %d\n", failedBytes);
  }
  else
  {
    printf("Cipher OK! YAY!\n");
  }

  return 0;
}