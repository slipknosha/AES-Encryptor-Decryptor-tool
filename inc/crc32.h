#pragma once

#define POLYNOMIAL 0x04c11db7L

typedef unsigned char byte;
typedef unsigned short int word16;
typedef unsigned int word32;

extern word32 crc_table[256];

void gen_crc_table(void);
word32 update_crc(word32 crc_accum, byte *data_blk_ptr, word32 data_blk_size);
