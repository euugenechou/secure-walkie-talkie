#include "being.h"
#include "dh.h"
#include "rsa.h"
#include "util.h"

Being::Being(std::string name_, BigInt rsa_p_, BigInt rsa_q_, BigInt rsa_e_,
    BigInt dh_p_, BigInt dh_g_, BigInt dh_k_) {
  name = name_;

  rsa_p = rsa_p_;
  rsa_q = rsa_q_;

  rsa_e = rsa_e_;
  rsa_n = RSA::generate_pub_key(rsa_p, rsa_q);
  rsa_d = RSA::generate_priv_key(rsa_p, rsa_q, rsa_e);

  dh_p = dh_p_;

  dh_g = dh_g_;
  dh_k = dh_k_;
  dh_pk = DiffieHellman::generate_shared_key(dh_g, dh_k, dh_p);
}

Being::Being(std::string name_, BigInt rsa_n_, BigInt rsa_e_) {
  name = name_;
  rsa_n = rsa_n_;
  rsa_e = rsa_e_;
}

BigInt Being::encrypt(const Being& other, const BigInt& m) {
 return RSA::encrypt(m, rsa_e, other.rsa_n);
}

BigInt Being::decrypt(const BigInt& c) {
  return RSA::decrypt(c, rsa_d, rsa_n);
}

BigInt Being::sign(const BigInt& m) {
  return RSA::sign(m, rsa_d, rsa_n);
}

BigInt Being::verify(const Being& other, const BigInt& s) {
  return RSA::verify(s, rsa_e, other.rsa_n);
}
