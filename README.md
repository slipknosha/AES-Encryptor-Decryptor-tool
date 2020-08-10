# task_

AES Encryptor/Decryptor tool

The goal of the task is to create a PC tool (with command line interface) to encrypt and decrypt binary file using AES-256 encryption algorithm with the following functionality and features:

  * It accepts a path to a binary image (\*.bin), output directory and an encryption key from a command line 
 
  * It encrypts and decrypts binary image (without the header) using AES-256 encryption algorithm
  * It appends a header with a magic number (i.e. “BEBEBEBE”), file size and calculated CRC32 to encrypted image
  * It prints the header on encryption completion
  * It prints the header on decryption start, after decryption compares newly calculated CRC with CRC in the header
  * Any C-based crypto library can be used (i.e. libtomcrypt, embedtls, etc)
  * Prints program usage (with “-h” key)
  * It is written in C programming language (C-99)
  * It runs under Linux environment (Cygwin, MinGW on Windows)

Acceptance criteria:
 * Project can be built from the scripts provided
 * The tool supports the specified functionality (some justified variations are acceptable) 

The work will be assessed based on the following points:
 * It is personal code
 * Function names/signatures, global/local variable declarations
 * Architectural decisions made
