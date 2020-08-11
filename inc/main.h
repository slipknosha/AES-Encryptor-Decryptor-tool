#ifndef _MAIN_H_
#define _MAIN_H_

typedef unsigned char byte;
typedef unsigned short int word16;
typedef unsigned int word32;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "aes.h"
#include "crc32.h"
#include "files.h"

#define ECB 1
#define ENCRYPT 1
#define DECRYPT 0
#define HELP 2

uint8_t par_mode(const uint8_t*);
int key_size(const uint8_t*);
void encrypt(uint8_t*, int, struct AES_ctx);
void decrypt(uint8_t*, int, struct AES_ctx);
uint8_t* read_header(const uint8_t*, int*, word32*, uint8_t);
void print_header(uint8_t*, int, word32);

#endif
