#ifndef __SIMON_H__
#define __SIMON_H__

#include "util.h"

void simon_key_schedule(u64 K[]);

void simon_encrypt(u64 Pt[], u64 Ct[]);

void simon_decrypt(u64 Ct[], u64 Pt[]);

#endif
