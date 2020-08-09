#ifndef _CRC32_H_
#define _CRC32_H_ 

#define POLYNOMIAL 0x04c11db7L

typedef unsigned char      byte;    // Byte is a char
typedef unsigned short int word16;  // 16-bit word is a short int
typedef unsigned int       word32;  // 32-bit word is an int

word32 crc_table[256]; 

void gen_crc_table(void);
word32 update_crc(word32 crc_accum, byte *data_blk_ptr, word32 data_blk_size);



#endif
