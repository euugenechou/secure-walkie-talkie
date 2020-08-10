#include "simon.h"

static u64 rk[72] = { 0 };

void simon_key_schedule(u64 K[]) {
  const u8 z[] = {
    1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0,
    0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1
  };

  rk[0] = K[0]; rk[1] = K[1]; rk[2] = K[2]; rk[3] = K[3];

  for (u8 i = 4; i < 72; i += 1) {
    u64 tmp = RR(rk[i - 1], 3);
    tmp = tmp ^ rk[i - 3];
    tmp = tmp ^ RR(tmp, 1);
    rk[i] = ~rk[i - 4] ^ tmp ^ z[(i - 4) % 62] ^ 3;
  }

  return;
}

void simon_encrypt(u64 Pt[], u64 Ct[]) {
  Ct[0] = Pt[0]; Ct[1] = Pt[1];

  for (u8 i = 0; i < 72; i += 1) {
    u64 tmp = Ct[1];
    Ct[1] = Ct[0] ^ (RL(Ct[1], 1) & RL(Ct[1], 8)) ^ RL(Ct[1], 2) ^ rk[i];
    Ct[0] = tmp;
  }

  return;
}

void simon_decrypt(u64 Ct[], u64 Pt[]) {
  Pt[0] = Ct[0]; Pt[1] = Ct[1];

  for (d8 i = 71; i >= 0; i -= 1) {
    u64 tmp = Pt[0];
    Pt[0] = Pt[1] ^ (RL(Pt[0], 1) & RL(Pt[0], 8)) ^ RL(Pt[0], 2) ^ rk[i];
    Pt[1] = tmp;
  }

  return;
}
