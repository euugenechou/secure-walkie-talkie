#include "util.h"
#include <chrono>

bool verbose = false;

u64 ms_since_epoch() {
  std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
  std::chrono::system_clock::duration dtn = tp.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(dtn).count();
}

void words_to_bytes(u64 words[], u8 bytes[], u8 nwords) {
  u8 j = 0;

  for (u8 i = 0; i < nwords; i += 1) {
    bytes[j] = (u8)words[i];
    bytes[j + 1] = (u8)(words[i] >> 8);
    bytes[j + 2] = (u8)(words[i] >> 16);
    bytes[j + 3] = (u8)(words[i] >> 24);
    bytes[j + 4] = (u8)(words[i] >> 32);
    bytes[j + 5] = (u8)(words[i] >> 40);
    bytes[j + 6] = (u8)(words[i] >> 48);
    bytes[j + 7] = (u8)(words[i] >> 56);
    j += 8;
  }

  return;
}

void bytes_to_words(u8 bytes[], u64 words[], u8 nbytes) {
  u8 j = 0;

  for (u8 i = 0; i < nbytes / 8; i += 1) {
    words[i] = (u64)bytes[j];
    words[i] |= ((u64)bytes[j + 1] << 8);
    words[i] |= ((u64)bytes[j + 2] << 16);
    words[i] |= ((u64)bytes[j + 3] << 24);
    words[i] |= ((u64)bytes[j + 4] << 32);
    words[i] |= ((u64)bytes[j + 5] << 40);
    words[i] |= ((u64)bytes[j + 6] << 48);
    words[i] |= ((u64)bytes[j + 7] << 56);
    j += 8;
  }

  return;
}
