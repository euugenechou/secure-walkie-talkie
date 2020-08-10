#include "rsa.h"

BigInt RSA::generate_pub_key(const BigInt& p, const BigInt& q) {
  return p * q;
}

BigInt RSA::generate_priv_key(
  const BigInt& p, const BigInt& q, const BigInt& e) {
  return e.invm((p - 1) * (q - 1));
}

BigInt RSA::encrypt(const BigInt& m, const BigInt& e, const BigInt& n) {
  return m.powm(e, n);
}

BigInt RSA::decrypt(const BigInt& c, const BigInt& d, const BigInt& n) {
  return c.powm(d, n);
}

BigInt RSA::sign(const BigInt& m, const BigInt& d, const BigInt& n) {
  return m.powm(d, n);
}

BigInt RSA::verify(const BigInt& s, const BigInt& e, const BigInt& n) {
  return s.powm(e, n);
}
