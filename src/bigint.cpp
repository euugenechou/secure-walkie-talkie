#include "bigint.h"
#include <algorithm>
#include <utility>
#include <iostream>

BigInt& BigInt::operator=(BigInt rhs) {
  std::swap(v, rhs.v);
  return *this;
}

bool BigInt::operator==(const BigInt& rhs) const {
  return mpz_cmp(v.get_mpz_t(), rhs.v.get_mpz_t()) == 0;
}

bool BigInt::operator!=(const BigInt& rhs) const {
  return !operator==(rhs);
}

bool BigInt::operator<(const BigInt& rhs) const {
  return mpz_cmp(v.get_mpz_t(), rhs.v.get_mpz_t()) < 0;
}

bool BigInt::operator>(const BigInt& rhs) const {
  return rhs.operator<(*this);
}

bool BigInt::operator<=(const BigInt& rhs) const {
  return !operator>(rhs);
}

bool BigInt::operator>=(const BigInt& rhs) const {
  return !operator<(rhs);
}

BigInt& BigInt::operator++() {
  mpz_add_ui(v.get_mpz_t(), v.get_mpz_t(), 1);
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt temp(*this);
  operator++();
  return temp;
}

BigInt BigInt::operator+() const {
  BigInt temp;
  mpz_abs(temp.v.get_mpz_t(), v.get_mpz_t());
  return temp;
}

BigInt BigInt::operator-() const {
  BigInt temp;
  mpz_neg(temp.v.get_mpz_t(), v.get_mpz_t());
  return temp;
}

BigInt BigInt::operator!() const {
  BigInt temp;
  mpz_com(temp.v.get_mpz_t(), v.get_mpz_t());
  return temp;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
  mpz_add(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
  return BigInt(*this) += rhs;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
  mpz_sub(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
  return BigInt(*this) -= rhs;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
  mpz_mul(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
  return BigInt(*this) *= rhs;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
  mpz_fdiv_q(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator/(const BigInt& rhs) const {
  return BigInt(*this) /= rhs;
}

BigInt& BigInt::operator%=(const BigInt& rhs) {
  mpz_mod(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator%(const BigInt& rhs) const {
  return BigInt(*this) %= rhs;
}

BigInt& BigInt::operator&=(const BigInt& rhs) {
  mpz_and(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator&(const BigInt& rhs) const {
  return BigInt(*this) &= rhs;
}

BigInt& BigInt::operator|=(const BigInt& rhs) {
  mpz_ior(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator|(const BigInt& rhs) const {
  return BigInt(*this) |= rhs;
}

BigInt& BigInt::operator^=(const BigInt& rhs) {
  mpz_xor(v.get_mpz_t(), v.get_mpz_t(), rhs.v.get_mpz_t());
  return *this;
}

BigInt BigInt::operator^(const BigInt& rhs) const {
  return BigInt(*this) ^= rhs;
}

BigInt& BigInt::operator<<=(const BigInt& rhs) {
  mpz_mul_2exp(v.get_mpz_t(), v.get_mpz_t(), mpz_get_ui(rhs.v.get_mpz_t()));
  return *this;
}

BigInt BigInt::operator<<(const BigInt& rhs) const {
  return BigInt(*this) <<= rhs;
}

BigInt& BigInt::operator>>=(const BigInt& rhs) {
  mpz_fdiv_q_2exp(v.get_mpz_t(), v.get_mpz_t(), mpz_get_ui(rhs.v.get_mpz_t()));
  return *this;
}

BigInt BigInt::operator>>(const BigInt& rhs) const {
  return BigInt(*this) >>= rhs;
}

BigInt BigInt::pow(const BigInt& p) const {
  BigInt temp;
  mpz_pow_ui(temp.v.get_mpz_t(), v.get_mpz_t(), mpz_get_ui(p.v.get_mpz_t()));
  return temp;
}

BigInt BigInt::powm(const BigInt& p, const BigInt& m) const {
  BigInt temp;
  mpz_powm(
    temp.v.get_mpz_t(), v.get_mpz_t(), p.v.get_mpz_t(), m.v.get_mpz_t()
  );
  return temp;
}

BigInt BigInt::invm(const BigInt& m) const {
  BigInt temp;
  mpz_invert(temp.v.get_mpz_t(), v.get_mpz_t(), m.v.get_mpz_t());
  return temp;
}

BigInt BigInt::sqrtm(const BigInt& p) const {
  BigInt q = p - 1;
  BigInt ss = 0;
  BigInt z = 2;
  BigInt c, r, t, m;

  if (powm((p - 1) / 2, p) != 1) {
    return 0;
  }

  while ((q & 1) == 0) {
    ss += 1;
    q >>= 1;
  }

  if (ss == 1) {
    return powm((p + 1) / 4, p);
  }

  while (z.powm((p - 1) / 2, p) != p - 1) {
    z += 1;
  }

  c = z.powm(q, p);
  r = powm((q + 1) / 2, p);
  t = powm(q, p);
  m = ss;

  for (;;) {
    BigInt e = 0;
    BigInt i = 0;
    BigInt b = c;
    BigInt zz = t;

    if (t == 1) {
      return r;
    }

    while (zz != 1 && i < (m - 1)) {
      zz = (zz * zz) % p;
      i += 1;
    }

    e = m - i - 1;

    while (e > 0) {
      b = (b * b) % p;
      e -= 1;
    }

    r = (r * b) % p;
    c = (b * b) % p;
    t = (t * c) % p;
    m = i;
  }

  return BigInt(-1);
}

void BigInt::setbit(const BigInt& i) {
  mpz_setbit(v.get_mpz_t(), mpz_get_ui(i.v.get_mpz_t()));
  return;
}

void BigInt::clrbit(const BigInt& i) {
  mpz_clrbit(v.get_mpz_t(), mpz_get_ui(i.v.get_mpz_t()));
  return;
}

int BigInt::getbit(const BigInt& i) const {
  return mpz_tstbit(v.get_mpz_t(), mpz_get_ui(i.v.get_mpz_t()));
}

size_t BigInt::bitwidth() const {
  return mpz_sizeinbase(v.get_mpz_t(), 2);
}

std::string BigInt::str_in_base(int base) const {
  char *s_ = new char[mpz_sizeinbase(v.get_mpz_t(), base) + 2]();
  mpz_get_str(s_, base, v.get_mpz_t());
  std::string s = std::string(s_);
  delete[] s_;
  return s;
}

std::vector<uint8_t> BigInt::to_bytes(const BigInt& n) {
  size_t nbytes = (mpz_sizeinbase(n.v.get_mpz_t(), 2) + 7) / 8;
  std::vector<uint8_t> v(nbytes);
  mpz_export(v.data(), &nbytes, 1, 1, 0, 0, n.v.get_mpz_t());
  v.resize(nbytes);
  return v;
}

BigInt BigInt::from_bytes(const std::vector<uint8_t>& vec) {
  BigInt temp;
  mpz_import(temp.v.get_mpz_t(), vec.size(), 1, 1, 0, 0, vec.data());
  return temp;
}

std::string BigInt::to_string(const BigInt& n) {
  char *s_ = new char[mpz_sizeinbase(n.v.get_mpz_t(), 16) / 2 + 2]();
  mpz_export(s_, NULL, 1, sizeof(char), 0, 0, n.v.get_mpz_t());
  std::string s = std::string(s_);
  delete[] s_;
  return s;
}

BigInt BigInt::from_string(const std::string& str) {
  BigInt temp;
  mpz_import(temp.v.get_mpz_t(), str.size(), 1, sizeof(char), 0, 0, str.data());
  return temp;
}

BigInt BigInt::rand_n_bits(const size_t bits, const size_t seed) {
  BigInt temp;
  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, seed);
  mpz_urandomb(temp.v.get_mpz_t(), state, bits);
  return temp;
}

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

std::string BigInt::to_base64(const BigInt& n) {
  std::string num_str = n.str_in_base(10);
  std::vector<uint8_t> data(num_str.begin(), num_str.end());
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

BigInt BigInt::from_base64(const std::string& data) {
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

  return BigInt(std::string(decoded.begin(), decoded.end()));
}

uint64_t BigInt::to_uint64(const BigInt& n) {
  return (uint64_t)mpz_get_ui(n.v.get_mpz_t());
}

BigInt BigInt::from_uint64(const uint64_t n) {
  BigInt temp;
  mpz_set_ui(temp.v.get_mpz_t(), n);
  return temp;
}

std::ostream& operator<<(std::ostream& os, const BigInt& n) {
  os << n.v;
  return os;
}

std::istream& operator>>(std::istream& is, BigInt& n) {
  is >> n.v;
  return is;
}
