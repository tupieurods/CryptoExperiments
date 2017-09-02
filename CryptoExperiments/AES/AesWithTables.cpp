#include <cstdio>
#include "AesWithTables.h"
#include "AesNaive.h"
#include "Helpers.h"

void CipherWithTables(unsigned char *in, unsigned char *out, unsigned int *w, const AESparam *aesParam)
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

  for(unsigned int i = 1; i < aesParam->Nr; i++)
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

void GenerateTables(unsigned int **aesT0, unsigned int **aesT1, unsigned int **aesT2, unsigned int **aesT3)
{
  *aesT0 = new unsigned int[256];
  *aesT1 = new unsigned int[256];
  *aesT2 = new unsigned int[256];
  *aesT3 = new unsigned int[256];

  for(unsigned int i = 0; i < 256; i++)
  {
    unsigned char s = SboxTable[i];
    (*aesT0)[i] = ConstructUint(X3(s), s, s, XTIME(s));
    (*aesT1)[i] = CROTATE((*aesT0)[i], 8);
    (*aesT2)[i] = CROTATE((*aesT0)[i], 16);
    (*aesT3)[i] = CROTATE((*aesT0)[i], 24);
  }
}

void PrintTable(unsigned int *table)
{
  printf("{\n");
  for(unsigned int i = 0; i < 256; i += 4)
  {
    printf("  ");
    for(int j = 0; j < 4; j++)
    {
      printf("0x%08XU,%c", table[i + j], j == 3 ? '\n' : ' ');
    }
  }
  printf("};\n\n");
}

void ComputeAndShowTables()
{
  unsigned int *aesT0 = nullptr, *aesT1 = nullptr, *aesT2 = nullptr, *aesT3 = nullptr;
  GenerateTables(&aesT0, &aesT1, &aesT2, &aesT3);
  printf("static unsigned int AesT0[256] =\n");
  PrintTable(aesT0);
  printf("static unsigned int AesT1[256] =\n");
  PrintTable(aesT1);
  printf("static unsigned int AesT2[256] =\n");
  PrintTable(aesT2);
  printf("static unsigned int AesT3[256] =\n");
  PrintTable(aesT3);
}