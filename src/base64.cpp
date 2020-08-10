#include "base64.h"

static size_t base64_encoded_length(size_t nbytes) {
  return 4 * ((nbytes + 2) / 3);
}

static size_t base64_decoded_length(size_t nbytes) {
  return nbytes / 4 * 3;
}

static const uint8_t base64_encode_table[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/'
};

static const uint8_t base64_decoding[256] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62,  0,  0,  0, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,
   0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,
   0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

std::string base64::encode(const std::vector<u8>& data) {
  std::string encoded(base64_encoded_length(data.size()), '=');

  const size_t moduli[3] = { 0, 2, 1 };

  size_t i = 0, j = 0;

  while (i < data.size()) {
    size_t x = (i < data.size()) ? data[i++] : 0;
    size_t y = (i < data.size()) ? data[i++] : 0;
    size_t z = (i < data.size()) ? data[i++] : 0;

    size_t t = (x << 16) + (y << 8) + z;

    encoded[j++] = static_cast<char>(base64_encode_table[(t >> 18) & 63]);
    encoded[j++] = static_cast<char>(base64_encode_table[(t >> 12) & 63]);
    encoded[j++] = static_cast<char>(base64_encode_table[(t >> 6)  & 63]);
    encoded[j++] = static_cast<char>(base64_encode_table[(t >> 0)  & 63]);
  }

  for (size_t k = 0; k < moduli[data.size() % 3]; k += 1) {
    encoded[base64_encoded_length(data.size()) - 1 - k] = '=';
  }

  return encoded;
}

std::vector<u8> base64::decode(const std::string& data) {
  size_t padding = count(data.begin(), data.end(), '=');
  size_t decoded_length = base64_decoded_length(data.size()) - padding;
  std::vector<uint8_t> decoded(decoded_length);

  size_t i = 0, j = 0;

  while (i < data.size()) {
    size_t w = (data[i] == '=') ? 0 & i++ : base64_decoding[static_cast<uint8_t>(data[i++])];
    size_t x = (data[i] == '=') ? 0 & i++ : base64_decoding[static_cast<uint8_t>(data[i++])];
    size_t y = (data[i] == '=') ? 0 & i++ : base64_decoding[static_cast<uint8_t>(data[i++])];
    size_t z = (data[i] == '=') ? 0 & i++ : base64_decoding[static_cast<uint8_t>(data[i++])];

    size_t t = (w << 18) + (x << 12) + (y << 6) + z;

    if (j < decoded_length) decoded[j++] = (t >> 16) & 255;
    if (j < decoded_length) decoded[j++] = (t >> 8)  & 255;
    if (j < decoded_length) decoded[j++] = (t >> 0)  & 255;
  }

  return decoded;
}
