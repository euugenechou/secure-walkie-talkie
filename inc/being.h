#ifndef __BEING_H__
#define __BEING_H__

#include "bigint.h"

class Being {
  private:
    BigInt rsa_p;     // RSA private prime p.
    BigInt rsa_q;     // RSA private prime q.
    BigInt rsa_phi;   // RSA (p - 1) * (q - 1).
    BigInt rsa_d;     // RSA private key exponent.
    BigInt dh_p;      // Diffie-Hellman public prime.
    BigInt dh_g;      // Diffie-Hellman generator, agreed shared key.
    BigInt dh_k;      // Diffie-Hellman key.

  public:
    std::string name; // The name of the Being.
    BigInt rsa_n;     // RSA public key.
    BigInt rsa_e;     // RSA public exponent e.
    BigInt dh_pk;     // Diffie-Hellman public key.

    Being() {}

    Being(
      std::string name_,
      BigInt rsa_p_, BigInt rsa_q_, BigInt rsa_e_,
      BigInt dh_p_, BigInt dh_g_, BigInt dh_k_
    );

    Being(std::string name_, BigInt rsa_n_, BigInt rsa_e_);

    BigInt encrypt(const Being& other, const BigInt& m);
    BigInt decrypt(const BigInt& c);
    BigInt sign(const BigInt& m);
    BigInt verify(const Being& other, const BigInt& s);
};

#endif
