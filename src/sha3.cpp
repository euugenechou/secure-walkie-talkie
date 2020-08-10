#include "sha3.h"

#define ROTL64(x, y)  (((x) << (y)) | ((x) >> (64 - (y))))

#define BYTE      8
#define ROUNDS    24
#define LANES     25
#define WIDTH     (1600 / BYTE)
#define CAPACITY  (512 / BYTE)
#define RATE      (WIDTH - CAPACITY)

static int absorbed = 0;
static int padpoint = 0;

static union {
  u64 words[LANES];
  u8 bytes[WIDTH];
} state;

static void theta(void) {
  u64 C[5] = { 0 };
  u64 D[5] = { 0 };

  for (int i = 0; i < 5; i += 1) {
    C[i]  = state.words[i];
    C[i] ^= state.words[i + 5];
    C[i] ^= state.words[i + 10];
    C[i] ^= state.words[i + 15];
    C[i] ^= state.words[i + 20];
  }

  for (int i = 0; i < 5; i += 1) {
    D[i] = C[(i + 4) % 5] ^ ROTL64(C[(i + 1) % 5], 1);
  }

  for (int i = 0; i < 5; i += 1) {
    for (int j = 0; j < 25; j += 5) {
      state.words[i + j] ^= D[i];
    }
  }

  return;
}

static void rho(void) {
  const u8 rotations[25] = {
     0,  1, 62, 28, 27,
    36, 44,  6, 55, 20,
     3, 10, 43, 25, 39,
    41, 45, 15, 21,  8,
    18,  2, 61, 56, 14
  };

  for (int i = 0; i < 25; i += 1) {
    state.words[i] = ROTL64(state.words[i], rotations[i]);
  }

  return;
}

static void pi(void) {
  const u8 switcheroo[25] = {
     0, 10, 20,  5, 15,
    16,  1, 11, 21,  6,
     7, 17,  2, 12, 22,
    23,  8, 18,  3, 13,
    14, 24,  9, 19,  4
  };

  u64 temp[25] = { 0 };

  for (int i = 0; i < 25; i += 1) {
    temp[i] = state.words[i];
  }

  for (int i = 0; i < 25; i += 1) {
    state.words[switcheroo[i]] = temp[i];
  }

  return;
}

static void chi(void) {
  u64 temp[5] = { 0 };

  for (int j = 0; j < 25; j += 5) {
    for (int i = 0; i < 5; i += 1) {
      temp[i] = state.words[i + j];
    }

    for (int i = 0; i < 5; i += 1) {
      state.words[i + j] ^= (~temp[(i + 1) % 5]) & temp[(i + 2) % 5];
    }
  }

  return;
}

static void iota(int r) {
  const u64 constants[24] = {
    0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
    0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
    0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
    0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
    0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
    0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
    0x8000000000008080, 0x0000000080000001, 0x8000000080008008
  };

  state.words[0] ^= constants[r];
  return;
}

static void keccak(void) {
  for (int i = 0; i < ROUNDS; i += 1) {
    theta(); rho(); pi(); chi(); iota(i);
  }

  return;
}

void SHA3::absorb(const std::vector<u8>& data) {
  for (size_t i = 0; i < data.size(); i += 1) {
    state.bytes[absorbed++] ^= data[i];

    if (absorbed == RATE) {
      keccak();
      absorbed = 0;
    }
  }

  padpoint = absorbed;
  return;
}

void SHA3::squeeze(std::vector<u8>& digest) {
  digest.resize(32);

  state.bytes[padpoint] ^= 0x06;
  state.bytes[RATE - 1] ^= 0x80;

  keccak();

  for (int i = 0; i < 32; i += 1) {
    digest[i] = state.bytes[i];
  }

  for (int i = 0; i < 25; i += 1) {
    state.words[i] = 0;
  }

  absorbed = 0;
  padpoint = 0;

  return;
}
