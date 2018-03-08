#include <cstring>
#include "sponge_raccoon.h"

// this file mostly copied from reference sponge.h(cpp) file
// I have done this because it would give me better understanding what is going on

/**
* Execute Blake2b's G function, with all 12 rounds.
*
* @param v     A 1024-bit (16 uint64_t) array to be processed by Blake2b's G function
*/
void blake2bLyra_raccoon(uint64_t *v)
{
  for(int TT = 0; TT < 12; TT++)
  {
    ROUND_LYRA_RACCOON(0);
  }
  /*ROUND_LYRA_RACCOON(0);
  ROUND_LYRA_RACCOON(1);
  ROUND_LYRA_RACCOON(2);
  ROUND_LYRA_RACCOON(3);
  ROUND_LYRA_RACCOON(4);
  ROUND_LYRA_RACCOON(5);
  ROUND_LYRA_RACCOON(6);
  ROUND_LYRA_RACCOON(7);
  ROUND_LYRA_RACCOON(8);
  ROUND_LYRA_RACCOON(9);
  ROUND_LYRA_RACCOON(10);
  ROUND_LYRA_RACCOON(11);*/
}

/**
* Initializes the Sponge State. The first 512 bits are set to zeros and the remainder
* receive Blake2b's IV as per Blake2b's specification. <b>Note:</b> Even though sponges
* typically have their internal state initialized with zeros, Blake2b's G function
* has a fixed point: if the internal state and message are both filled with zeros. the
* resulting permutation will always be a block filled with zeros; this happens because
* Blake2b does not use the constants originally employed in Blake2 inside its G function,
* relying on the IV for avoiding possible fixed points.
*
* @param state         The 1024-bit array to be initialized
*/
void initState_raccoon(uint64_t state[/*16*/])
{
  //First 512 bis are zeros
  memset(state, 0, 64);
  //Remainder BLOCK_LEN_BLAKE2_SAFE_BYTES are reserved to the IV
  state[8] = blake2b_IV_raccoon[0];
  state[9] = blake2b_IV_raccoon[1];
  state[10] = blake2b_IV_raccoon[2];
  state[11] = blake2b_IV_raccoon[3];
  state[12] = blake2b_IV_raccoon[4];
  state[13] = blake2b_IV_raccoon[5];
  state[14] = blake2b_IV_raccoon[6];
  state[15] = blake2b_IV_raccoon[7];
}

/**
* Performs an absorb operation for a single block (BLOCK_LEN_BLAKE2_SAFE_INT64
* words of type uint64_t), using Blake2b's G function as the internal permutation
*
* @param state The current state of the sponge
* @param in    The block to be absorbed (BLOCK_LEN_BLAKE2_SAFE_INT64 words)
*/
void absorbBlockBlake2Safe_raccoon(uint64_t *state, const uint64_t *in)
{
  //XORs the first BLOCK_LEN_BLAKE2_SAFE_INT64 words of "in" with the current state

  state[0] ^= in[0];
  state[1] ^= in[1];
  state[2] ^= in[2];
  state[3] ^= in[3];
  state[4] ^= in[4];
  state[5] ^= in[5];
  state[6] ^= in[6];
  state[7] ^= in[7];


  //Applies the transformation f to the sponge's state
  blake2bLyra_raccoon(state);
}

/**
* Executes a reduced version of Blake2b's G function with only one round
* @param v     A 1024-bit (16 uint64_t) array to be processed by Blake2b's G function
*/
void reducedBlake2bLyra_raccoon(uint64_t *v)
{
  ROUND_LYRA_RACCOON(0);
}
