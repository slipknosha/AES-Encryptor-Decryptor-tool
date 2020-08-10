#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if(argc != 3) {
        puts("Format is wrong\n");
        return 0;
    }

    int char_amount = 0;
    for(char* ptr = argv[2]; *ptr != '\0'; ++ptr) {
        char_amount *= 10;
        char_amount += *ptr - 48;
    }
    const char* file_name = argv[1];


    FILE* bin_file = fopen(file_name,"wb");
    if(bin_file == NULL) {
        puts("File hasn't opened\n");
        return 0;
    }

    for(int i = 0; i < char_amount; ++i) {
        int symbol = rand() % 128;
        fputc(symbol, bin_file);
    }
    fclose(bin_file);
    return 0;
} 
