#include "../inc/main.h"
//extern word32 crc_table[];

int main(int argc, char* argv[])
{
  const uint8_t* mode = argv[1];
  if (argc != 4) {
    if (argc == 2 && (par_mode(mode) == HELP)) {
      puts("\n1. The first parameter is the mode: encryption or decryption and call the help:\n"
        "-e or --encrypt\n"
        "-d or --decrypt\n"
        "-h or --help\n"
        "2. The second parameter is the file name.The path cannot be absolute,\n"
        "   so the binary file can be located only in the same directory with the program.\n"
        "3. The last parameter is a 32 byte key, which must be entered as characters.\n");
      return 0;
    }
    puts("Not enough arguments!\n"
      "Write ./task -h or task --help");
    return 0;
  }

  if (key_size(argv[3]) != AES_KEYLEN) {
    puts("Key must contain 32 bytes!");
    return 0;
  }

  uint8_t* file_name = argv[2];
  const uint8_t* key = argv[3];
  struct AES_ctx ctx;
  int f_size;
  word32 crc_before=0;
  word32 crc_after=0;
  uint8_t* buf;
  AES_init_ctx(&ctx, key);
  gen_crc_table();

  printf("\nkey in ASCII: %s\n", key);
  puts("\nkey in HEX:");
  show_file(key, 32);

  if (par_mode(mode) == ENCRYPT) {
    buf = read_file(file_name, &f_size);
    puts("\nFile before encrypt:");
    show_file(buf, f_size);
    crc_before = update_crc(-1, buf, f_size);
    printf("\nCRC32: 0x%X\n", crc_before);
    encrypt(buf, f_size, ctx);
    puts("\nFile after encrypt:");
    show_file(buf, f_size);
    file_name = write_file(file_name, buf, f_size, crc_before, ENCRYPT);
    read_header(file_name, 0, 0, ENCRYPT);
    free(file_name);
    return 0;
  }

  if (par_mode(mode) == DECRYPT) {
    buf = read_file(file_name, &f_size);
    puts("\nFile before decrypt:");
    show_file(buf, f_size);
    free(buf);
    buf = read_header(file_name, &f_size, &crc_before, DECRYPT);
    decrypt(buf, f_size, ctx);
    puts("\nfile after decrypt:");
    show_file(buf, f_size);
    crc_after = update_crc(-1, buf, f_size);
    printf("\nCRC32: 0x%X\n", crc_after);
    if (crc_before == crc_after) {
      puts("CRC in HEADER is equal recalculated CRC after decrypt.");
    }
    write_file(file_name, buf, f_size, 0, DECRYPT);
    return 0;
  }
  return 0;
}

uint8_t par_mode(const uint8_t* par)
{
  if (!strcmp(par, "-h") || !strcmp(par, "--help")) {
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

int key_size(const uint8_t* key)
{
  int size = 0;
  uint8_t* k = key;
  while (*k != '\0') {
    k++;
    size++;
  }
  return size;
}

void encrypt(uint8_t* buf, int f_size, struct AES_ctx ctx)
{
  for (int i = 0; i < f_size / 16; ++i) {
    AES_ECB_encrypt(&ctx, buf + (i * 16));
  }
}

void decrypt(uint8_t* buf, int f_size, struct AES_ctx ctx)
{
  for (int i = 0; i < f_size / 16; ++i) {
    AES_ECB_decrypt(&ctx, buf + (i * 16));
  }
}

uint8_t* read_header(const uint8_t* name, int* size, word32* CRC32, uint8_t mod)
{
  FILE* fp;
  if ((fp = fopen(name, "rb")) == NULL)
  {
    perror("Error occured while opening file");
    exit(1);
  }

  uint8_t* buf = NULL;
  char mag_num[9];
  mag_num[8] = '\0';

  word32 crc_temp;
  int size_temp;

  fread(&crc_temp, sizeof(*CRC32), 1 , fp);
  fread(&size_temp, sizeof(*size), 1 , fp);
  fread(mag_num, sizeof(char), 8, fp);

  if (!mod) {
    buf = malloc(size_temp);
    fread(buf, 1, size_temp, fp);
  }
  fclose(fp);
  
  print_header(mag_num, size_temp, crc_temp);

  if (!mod) {
    *size = size_temp;
    *CRC32 = crc_temp;
    return buf;
  }

  return NULL;
}

void print_header(uint8_t* mag_num, int size, word32 CRC32)
{
  printf("\nHEADER\nMagic number: %s\nSize: 0x%X\nCRC: 0x%X\n", mag_num, size, CRC32);
}
