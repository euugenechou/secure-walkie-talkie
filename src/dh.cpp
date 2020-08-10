#include "dh.h"

BigInt DiffieHellman::generate_shared_key(
    const BigInt& g, const BigInt& a, const BigInt& p) {
  return g.powm(a, p);
}
