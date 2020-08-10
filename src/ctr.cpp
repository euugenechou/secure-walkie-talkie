#include "ctr.h"
#include "simon.h"

void ctr_encrypt(u64 nonce, u8 key[], u8 pt[], u8 ct[], u64 num_bytes) {
  u64 K[4] = { 0 };
  bytes_to_words(key, K, 32);

  simon_key_schedule(K);

  u64 Pk[2]  = { nonce, 0 };
  u64 Ck[2]  = { 0 };
  u8  ck[16] = { 0 };

  u64 blocks = num_bytes / 16;
  u64 bytes  = num_bytes % 16;

  for (u64 i = 0; i < blocks; i += 1) {
    Pk[1] += 1;
    simon_encrypt(Pk, Ck);
    words_to_bytes(Ck, ck, 2);

    for (u8 j = 0; j < 16; j += 1) {
      ct[16 * i + j] = pt[16 * i + j] ^ ck[j];
    }
  }

  Pk[1] += 1;
  simon_encrypt(Pk, Ck);
  words_to_bytes(Ck, ck, 2);

  for (u64 i = 0; i < bytes; i += 1) {
    ct[16 * blocks + i] = pt[16 * blocks + i] ^ ck[i];
  }

  return;
}

void ctr_decrypt(u64 nonce, u8 key[], u8 ct[], u8 pt[], u64 num_bytes) {
  ctr_encrypt(nonce, key, ct, pt, num_bytes);
  return;
}
