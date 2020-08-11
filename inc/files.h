#ifndef _FILES_H_
#define _FILES_H_

#include "main.h"

int file_size(FILE* fp);
uint8_t* read_file(const uint8_t*, int*);
uint8_t* write_file(uint8_t*, const uint8_t*, int, word32, uint8_t);
void show_file(const uint8_t*, int);
uint32_t bytes(uint8_t*);

#endif
