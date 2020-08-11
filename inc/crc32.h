#ifndef _CRC32_H_
#define _CRC32_H_

#include "main.h"

#define POLYNOMIAL 0x04c11db7L

word32 crc_table[256];

void gen_crc_table(void);
word32 update_crc(word32, byte*, word32);

#endif