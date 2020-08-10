#ifndef __SHA3_H__
#define __SHA3_H__

#include "util.h"
#include <vector>

namespace SHA3 {
  void absorb(const std::vector<u8>& data);
  void squeeze(std::vector<u8>& digest);
}

#endif
