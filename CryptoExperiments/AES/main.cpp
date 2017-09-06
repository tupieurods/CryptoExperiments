#include <cstdio>
#include <cstdlib>
#include "TestData.h"
#include "AesCommon.h"
#include "AesNaive.h"
#include "AesWithTables.h"

int main()
{
  //ComputeAndShowTables();

  const AESparam *selectedAesParam = &AES128param;
  const size_t wSize = (selectedAesParam->Nr + 1) * selectedAesParam->Nb;
  unsigned int *w = new unsigned int[wSize];
  KeyExpansion(cipherKey128, w, selectedAesParam);

  /*for(size_t i = 0; i < wSize; i++)
  {
    printf("w[%d] = 0x%08x\n", i, w[i]);
  }*/

  unsigned char cipherOutput[16];

  //Cipher(cipherInput, cipherOutput, w, selectedAesParam);
  CipherWithTables(cipherInput, cipherOutput, w, selectedAesParam);

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