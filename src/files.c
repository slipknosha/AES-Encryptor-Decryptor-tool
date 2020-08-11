#include "../inc/files.h"

int file_size(FILE* fp)
{
	int save_pos, size_of_file;
	save_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	fseek(fp, save_pos, SEEK_SET);
	return size_of_file;
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
	for (int i = 0; i < f_size; ++i) {
		putc('\t', stdout);
		for (int j = 0; j < 8; ++j, ++i) {
			printf("0x%-2X ", buf[i]);
		}
		putc('\n', stdout);
	}
}

uint8_t* write_file(uint8_t* name, const uint8_t* buf, int size, word32 CRC32, uint8_t mod)
{
	FILE* fp;
	uint32_t ar_bytes = bytes(name);
	uint8_t* f_name = malloc(ar_bytes + 3);
	uint8_t* ptr_name = name;

	if (mod) {
		f_name[0] = 'e';
		f_name[1] = 'n';
		for (int i = 2; i < ar_bytes + 1; i++) {
			f_name[i] = *ptr_name++;
		}

		f_name[ar_bytes + 1] = '\0';
		fp = fopen(f_name, "wb");
		if (fp == NULL) {
			perror("Error occured while opening file");
			exit(1);
		}
	}
	else {
		f_name[0] = 'd';
		f_name[1] = 'e';
		f_name[ar_bytes] = '\0';

		fp = fopen(f_name, "wb");
		if (fp == NULL) {
			perror("Error occured while opening file");
			exit(1);
		}
	}

	if (mod) {
		fputs("BEBEBEBE", fp);
		uint8_t* size_ptr = &size;
		uint8_t* crc32_ptr = &CRC32;

		for (int i = 0; i < sizeof(size); ++i) {
			putc(*size_ptr, fp);
			++size_ptr;
		}
		for (int i = 0; i < sizeof(word32); ++i) {
			putc(*crc32_ptr, fp);
			++crc32_ptr;
		}
	}

	uint8_t* ptr = buf;
	for (int i = 0; i < size; ++i) {
		putc(*ptr, fp);
		++ptr;
	}

	fclose(fp);
	return f_name;
}

uint32_t bytes(uint8_t* arr)
{
	uint32_t size = 0;
	uint8_t* ptr = arr;
	while (*ptr++ && ++size);
	return size + 1;
}
