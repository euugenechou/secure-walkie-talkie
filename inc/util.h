#ifndef __UTIL_H__
#define __UTIL_H__

#include <cinttypes>
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>

#define SERVER_PORT   8123
#define NONCE         1234

#define REQUEST       '1'
#define RESPONSE      '2'
#define HMAC          '3'
#define AUDIO         'D'

#define RL(x, y)  (((x) << (y)) | (x >> (64 - (y))))
#define RR(x, y)  (((x) >> (y)) | ((x) << (64 - (y))))

using JSON = nlohmann::json;

typedef int8_t    d8;
typedef int16_t   d16;
typedef int32_t   d32;
typedef int64_t   d64;

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;


extern bool verbose;

#define LOG(CODE) {                   \
  if (verbose) {                      \
    std::cerr << CODE << std::endl;   \
  }                                   \
}

#define LOGNN(CODE) {     \
  if (verbose) {          \
    std::cerr << CODE;    \
  }                       \
}

#define CHECK(COND, CODE) {           \
  if (!(COND)) {                      \
    std::cerr << CODE << std::endl;   \
    exit(EXIT_FAILURE);               \
  }                                   \
}

u64 ms_since_epoch();

void words_to_bytes(u64 words[], u8 bytes[], u8 nwords);

void bytes_to_words(u8 bytes[], u64 words[], u8 nbytes);

#endif
