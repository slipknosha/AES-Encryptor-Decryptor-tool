#include "../inc/crc32.h"

word32 crc_table[256];
//===== Main program ==========================================================
/*void main(void)
{
	byte        buff[BUFFER_LEN]; // Buffer of packet bytes
	word32      crc32;            // 32-bit CRC value
	word16      i;                // Loop counter (16 bit)
	word32      j;                // Loop counter (32 bit)

	// Initialize the CRC table
	gen_crc_table();

	// Load buffer with BUFFER_LEN random bytes
	for (i=0; i<BUFFER_LEN; i++)
		buff[i] = (byte) rand();

	// Compute and output CRC
	crc32 = update_crc(-1, buff, BUFFER_LEN);
	printf("CRC = %08X \n", crc32);
}
*/
//=============================================================================
//=  CRC32 table initialization                                               =
//=============================================================================
void gen_crc_table(void)
{
	register word16 i, j;
	register word32 crc_accum;

	for (i = 0; i < 256; i++)
	{
		crc_accum = ((word32)i << 24);
		for (j = 0; j < 8; j++)
		{
			if (crc_accum & 0x80000000L)
				crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
			else
				crc_accum = (crc_accum << 1);
		}
		crc_table[i] = crc_accum;
	}
}

//=============================================================================
//=  CRC32 generation                                                         =
//=============================================================================
word32 update_crc(word32 crc_accum, byte* data_blk_ptr, word32 data_blk_size)
{
	register word32 i, j;

	for (j = 0; j < data_blk_size; j++)
	{
		i = ((int)(crc_accum >> 24) ^ *data_blk_ptr++) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	crc_accum = ~crc_accum;

	return crc_accum;
}