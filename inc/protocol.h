#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "bigint.h"
#include "util.h"
#include <cstdbool>

namespace protocol {
  JSON create_request(const BigInt& alice_rsa_d, const BigInt& alice_rsa_n, const
      BigInt& bob_rsa_e, const BigInt& bob_rsa_n, const BigInt& dh_g, const
      BigInt& dh_p, BigInt& tod, BigInt& alice_dh_pri);

  bool verify_request(const JSON& request, const BigInt& bob_rsa_d, const
      BigInt& bob_rsa_n, const JSON& contacts, BigInt& tod, BigInt&
      alice_dh_pub, BigInt& alice_rsa_e, BigInt& alice_rsa_n);

  JSON create_response(const BigInt& bob_rsa_d, const BigInt& bob_rsa_n,
      const BigInt& alice_rsa_e, const BigInt& alice_rsa_n, const BigInt&
      alice_dh_pub, const BigInt& dh_g, const BigInt& dh_p, const BigInt& tod,
      BigInt& k1, BigInt& k2);

  bool verify_response(const JSON& response, const BigInt& alice_rsa_d,
      const BigInt& alice_rsa_n, const BigInt& bob_rsa_e, const BigInt&
      bob_rsa_n, const BigInt& dh_p, const BigInt& alice_dh_pri, const BigInt&
      tod, BigInt& k1, BigInt& k2);

  void encrypt_audio(std::string ptfname, std::string ctfname, BigInt& tod,
      BigInt& k1);

  void decrypt_audio(std::string ctfname, std::string pftname, BigInt& tod,
      BigInt& k1);

  JSON create_tag(std::string audiofname, const BigInt& k2);

  bool verify_tag(const JSON& tag, std::string audiofname, const BigInt& k2);
}

#endif
