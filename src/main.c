#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ECB 1
#define ENCRYPT 1
#define DECRYPT 0
#define HELP 2

#include "../inc/aes.h"
#include "../inc/crc32.h"

uint8_t par_mode(const uint8_t*);
int file_size(FILE* fp);
int key_size(uint8_t*);
uint8_t* read_file(const uint8_t*, int*);
int write_file(const uint8_t*, int, word32);
void show_file(const uint8_t*, int);

static void phex(uint8_t* str)
{
	uint8_t len = 32;
	uint8_t i;
	for (i = 0; i < len; ++i)
		printf("%.2x", str[i]);
	printf("\n");
}

int main(int argc, char* argv[])
{
	argc = 4;
	argv[1] = "--encrypt";
	argv[2] = "list.bin";
	argv[3] = "gldwp4235tykxmqajrtyopmcterk7319";
	if (argc != 4) {
		puts("Format is wrong!\n"
			"Write ./task -h or task --help");
		return 0;
	}
	if (key_size(argv[3]) != AES_KEYLEN) {
		puts("Key must be contain 32 bytes!");
		return 0;
	}
	const uint8_t* mode = argv[1];
	//uint8_t* mode = argv[1];
	const uint8_t* file_name = argv[2];
	const uint8_t* key = argv[3];
	int f_size;
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);
	uint8_t* buf = read_file(file_name, &f_size);
	gen_crc_table();
	word32 crc_before = update_crc(-1, buf, f_size);
	printf("file before encrypt:\n");
	show_file(buf, f_size);
	printf("\nkey in ASCII: %s\n", key);
	printf("key in HEX:");
	show_file(key, 32);
	printf("\nCRC32:0x%X\n", crc_before);
	if (par_mode(mode) == ENCRYPT) {
		for (int i = 0; i < f_size / 16; ++i) {
			AES_ECB_encrypt(&ctx, buf + (i * 16));
		}
		show_file(buf, f_size);
		write_file(buf, f_size, crc_before);
	}
	//puts("\n\n");
	//mode = "--decrypt";
	//if (par_mode(mode) == DECRYPT) {
	//	for (int i = 0; i < f_size / 32 + 1; ++i) {
	//		AES_ECB_decrypt(&ctx, buf + (i * 32));
	//	}
	//	show_file(buf, f_size);
	//}
	free(buf);
	return 0;
}

int file_size(FILE* fp)
{
	int save_pos, size_of_file;
	save_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	fseek(fp, save_pos, SEEK_SET);
	return size_of_file;
}

uint8_t par_mode(const uint8_t* par)
{
	if (!strcmp(par, "-h") || !strcmp(par, "--help")) {
		puts("help");
		return HELP;
	}
	if (!strcmp(par, "-e") || !strcmp(par, "--encrypt")) {
		return ENCRYPT;
	}
	if (!strcmp(par, "-d") || !strcmp(par, "--decrypt")) {
		return DECRYPT;
	}
	return -1;
}

int key_size(uint8_t* key)
{
	int size = 0;
	uint8_t* k = key;
	while (*k != '\0') {
		k++;
		size++;
	}
	return size;
}

uint8_t* read_file(const uint8_t* f_name, int* f_size)
{
	FILE* fp;
	fp = fopen(f_name, "rb");
	if (fp == NULL) {
		perror("Error occured while opening file");
		exit(1);
	}
	*f_size = file_size(fp);
	uint8_t* buf = malloc(*f_size + 1);
	fread(buf, 1, *f_size, fp);
	fclose(fp);
	return buf;
}

void show_file(const uint8_t* buf, int f_size)
{
	for (int i = 0; i < f_size; i++) {
		printf("0x%X ", buf[i]);
	}
}

int write_file(const uint8_t* buf, int size, word32 CRC32)
{
	//word32 counter = 8 + sizeof(word32) + sizeof(long int);
	word32 counter = sizeof("BEBEBEBE") + sizeof(size) + sizeof(CRC32) + size;
	FILE* fp;
	if ((fp = fopen("enlist.bin", "wb")) == NULL)
	{
		perror("Error occured while opening file");
		return 1;
	}
	fputs("BEBEBEBE", fp);

	uint8_t* size_ptr = &size;
	uint8_t* crc32_ptr = &CRC32;

	//fprintf(fp, "%x%x", CRC32, size);
	for (int i = 0; i < sizeof(size); ++i) {
		putc(*size_ptr, fp);
		++size_ptr;
	}
	for (int i = 0; i < sizeof(word32); ++i) {
		putc(*crc32_ptr, fp);
		++crc32_ptr;
	}
	uint8_t* ptr = buf;
	for (int i = 0; i < size; ++i) {
		putc(*ptr, fp);
		++ptr;
	}
	fclose(fp);
	return counter;
}
