#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ECB 1

#include "aes.h"
#include "crc32.h"

int check_param(const uint8_t*);
int file_size(FILE *fp);
int key_size(uint8_t*);
//char* read_file(const char*, long int*);
void show_file(uint8_t*, long int);

static void phex(uint8_t* str)
{
    uint8_t len = 32;
    uint8_t i;
    for (i = 0; i < len; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}

int main(int argc, uint8_t* argv[])
{
   argc = 4;
  argv[1] = "--encrypt";
   argv[2] = "C:\\Users\\vikaf\\source\\repos\\task\\Debug\\list.bin";
    argv[3] = "absrytkgmbieqwsdfmgtp[lmqwadgmnt";
    if (argc != 4) {
        puts("Format is wrong!\n"
            "Write ./task -h or task --help");
        return 0;
    }
  if(key_size(argv[3])!=32) {
    puts("Key must be contain 32 bytes!");
    return 0;
  }
  long int f_size;
  const uint8_t* file_name = argv[2];
  const uint8_t* key = argv[3];
  FILE* fp;
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
      perror("Error occured while opening file");
      exit(1);
  }
  f_size = file_size(fp);
  uint8_t buf[256];
  fread(buf, 1, f_size, fp);
  word32 crc_before = update_crc(-1, buf, f_size);
  printf("file before encrypt:\n");
  show_file(buf, f_size);
  printf("\nkey: %s\n", key);
  
  if(check_param(argv[1])=='1') {
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);
    for(int i=0;i<(f_size/4);++i) {
        AES_ECB_encrypt(&ctx, buf + i);
      //  phex(buf + (i * 16));
    }
    //printf("\nfile after encrypt");
    show_file(buf, f_size);
  }
  //free(buf);
  return 0;
}

long int file_size(FILE *fp)
{
  long int save_pos, size_of_file;
  save_pos = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  size_of_file = ftell(fp);
  fseek(fp, save_pos, SEEK_SET);
  return size_of_file;
}

int check_param(const uint8_t* par)
{
  if(!strcmp(par, "-h") || !strcmp(par, "--help")) {
    puts("help");
    return '-1';
  }
  if(!strcmp(par, "-e") || !strcmp(par, "--encrypt")) {
    return '1';
  }
  if(!strcmp(par, "-d") || !strcmp(par, "--decrypt")) {
    return '0';
  }
}

int key_size(uint8_t* key)
{
  int size=0;
  uint8_t* k = key;
  while(*k!='\0') {
      k++;
    size++;
  }
  return size;
}

void read_file(uint8_t* buf,const uint8_t* f_name, long int* f_size)
{
    FILE* fp;
    fp = fopen(f_name, "rb");
    if (fp == NULL) {
		perror("Error occured while opening file");
        exit(1);
	}
    *f_size = file_size(fp);
   // char* buffer = malloc(*f_size);
    fread(buf, 1, *f_size, fp);
    /*for (char* ptr = buffer; *ptr != EOF; ++ptr) {
        *ptr = getc(fp);
    }*/
    //while ((*ptr != EOF)) {
    //    *ptr = getc(fp);
    //    ptr++;
    //}
	fclose(fp);
}

void show_file(uint8_t* buf, long int f_size)
{
  for(int i = 0; i < f_size; i++) {
    printf("%X", buf[i]);
  }
}
