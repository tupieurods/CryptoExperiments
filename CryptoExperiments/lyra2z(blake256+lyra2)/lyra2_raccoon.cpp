#include <cstring>
#include <cstdio>
#include "lyra2_raccoon.h"
#include "sponge_raccoon.h"
//#include "lyra2.h"

struct Lyra2MatrixElement
{
  uint64_t item[12];
};

void printSpongeState_raccoon(uint64_t *spongeState)
{
  printf("Lyra2_32_32_8_8_8 state:\n");
  for(int i = 0; i < 16; i++)
  {
    printf("0x%016llx\n", spongeState[i]);
  }
}

void printMatrixRow_raccoon(Lyra2MatrixElement *matrix)
{
  printf("Lyra2_32_32_8_8_8 matrix row:\n");
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 12; j++)
    {
      printf("0x%016llx_", matrix[i].item[j]);
    }
    printf("\n");
  }
}

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
  //printSpongeState_raccoon(spongeState);
  // END Bootstrapping phase: init sponge state
  Lyra2MatrixElement matrix[8][8];
  memset(matrix, 0, sizeof matrix);
  // BEGIN Setup phase.

  // for (col 0 to C-1) do {M[0][C-1-col] = H.reduced_squeeze(b)} Initializes M[0]
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 12; j++)
    {
      matrix[0][7 - i].item[j] = spongeState[j];
    }
    reducedBlake2bLyra_raccoon(spongeState);
  }
  //printMatrixRow_raccoon(matrix[0]);

  // for (col 0 to C-1) do {M[1][C-1-col] = M[0][col] ^ H.reduced_duplex(M[0][col], b)} Initializes M[1] and updates state
  for(int i = 0; i < 8; i++)
  {
    //Absorbing "M[prev][col]"
    for(int j = 0; j < 12; j++)
    {
      spongeState[j] ^= matrix[0][i].item[j];
    }
    //Applies the reduced-round transformation f to the sponge's state
    reducedBlake2bLyra_raccoon(spongeState);
    //M[row][C-1-col] = M[prev][col] XOR rand
    for(int j = 0; j < 12; j++)
    {
      matrix[1][7 - i].item[j] = matrix[0][i].item[j] ^ spongeState[j];
    }
  }
  //printSpongeState_raccoon(spongeState);
  //printMatrixRow_raccoon(matrix[1]);

  // Filling loop
  int64_t row0 = 2; //index of row to be processed
  int64_t prev0 = 1; //index of prev (last row ever computed/modified)
  int64_t row1 = 0; //index of row* (a previous row, deterministically picked during Setup and randomly picked while Wandering)
  int64_t step = 1; //Visitation step (used during Setup and Wandering phases)
  int64_t window = 2; //Visitation window (used to define which rows can be revisited during Setup)
  int64_t gap = 1; //Modifier to the step, assuming the values 1 or -1
  do
  {
    for(int i = 0; i < 8; i++)
    {
      for(int j = 0; j < 12; j++)
      {
        spongeState[j] ^= matrix[prev0][i].item[j] + matrix[row1][i].item[j];
      }
      reducedBlake2bLyra_raccoon(spongeState);
      for(int j = 0; j < 12; j++)
      {
        matrix[row0][7 - i].item[j] = spongeState[j] ^ matrix[prev0][i].item[j];
      }
      for(int j = 0; j < 12; j++)
      {
        matrix[row1][i].item[j] ^= spongeState[(11 + j) % 12];
      }
    }

    //updates the value of row* (deterministically picked during Setup))
    row1 = (row1 + step) & (window - 1);
    //update prev: it now points to the last row ever computed
    prev0 = row0;
    //updates row: goes to the next row to be computed
    row0++;

    //Checks if all rows in the window where visited.
    if(row1 == 0) {
      step = window + gap; //changes the step: approximately doubles its value
      window *= 2; //doubles the size of the re-visitation window
      gap = -gap; //inverts the modifier to the step
    }
    printMatrixRow_raccoon(matrix[prev0]);
  } while(row0 < 8);

  // END Setup phase.

  //LYRA2(state, 32, input, 32, input, 32, 8, 8, 8);
}
