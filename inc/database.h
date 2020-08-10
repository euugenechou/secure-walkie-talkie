#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "being.h"
#include <string>
#include <vector>

extern std::vector<Being> beings;

namespace Database {
  void init();
  bool search(std::string name, Being& b);
}

#endif
