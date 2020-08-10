#ifndef __CTR_H__
#define __CTR_H__

#include "util.h"

void ctr_encrypt(u64 nonce, u8 key[], u8 pt[], u8 ct[], u64 num_bytes);

void ctr_decrypt(u64 nonce, u8 key[], u8 ct[], u8 pt[], u64 num_bytes);

#endif
