#ifndef __RSA_H__
#define __RSA_H__

#include "bigint.h"

namespace RSA {
  BigInt generate_pub_key(const BigInt& p, const BigInt& q);
  BigInt generate_priv_key(const BigInt& p, const BigInt& q, const BigInt& e);
  BigInt encrypt(const BigInt& m, const BigInt& e, const BigInt& n);
  BigInt sign(const BigInt& m, const BigInt& d, const BigInt& n);
  BigInt decrypt(const BigInt& c, const BigInt& d, const BigInt& n);
  BigInt verify(const BigInt& s, const BigInt& e, const BigInt& n);
}

#endif
