#include <cstdio>
#include "TestData.h"
#include "lyra2z.h"
#include "lyra2z_raccoon.h"
#include <cstring>

int main()
{
  unsigned char state[32], state2[32];
  memset(state, 0, sizeof(unsigned char) * 32);
  memset(state2, 0, sizeof(unsigned char) * 32);
  lyra2Zhash(state, testInput);
  Lyra2ZhashRaccoon(state2, testInput);

  int failedBytes = 0;
  for(int i = 0; i < 32; i++)
  {
    if(state[i] != state2[i])
    {
      failedBytes++;
    }
    printf("0x%02x 0x%02x %s\n", state[i], state2[i], state[i] != state2[i] ? "<--" : "");
  }
  printf("\n");
  if(failedBytes)
  {
    printf("Total bytes failed: %d. BOOOOO...\n", failedBytes);
  }
  else
  {
    printf("Hashes are equal! YAY!\n");
  }

  return 0;
}
