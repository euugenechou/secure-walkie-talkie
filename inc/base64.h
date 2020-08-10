#ifndef __BASE64_H__
#define __BASE64_H__

#include "util.h"

namespace base64 {
  std::string encode(const std::vector<u8>& data);
  std::vector<u8> decode(const std::string& data);
}

#endif
