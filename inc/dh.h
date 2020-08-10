#ifndef __DH_H__
#define __DH_H__

#include "bigint.h"

namespace DiffieHellman {
  BigInt generate_shared_key(
    const BigInt& g, const BigInt& a, const BigInt& p
  );
}

#endif
