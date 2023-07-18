
#include <err.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int compareUint16(const void* a, const void* b){
    uint16_t num1 = *(const uint16_t*)a;
    uint16_t num2 = *(const uint16_t*)b;

    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
}
int main(int argc, char* argv[]){
    if(argc != 3){
        errx(2,"We need 2 arguments");
    }
    int input=open(argv[1],O_RDONLY);
    if(input == -1){
        err(2,"error while opening");
    }
    int output=open(argv[2],O_CREAT|O_TRUNC| O_WRONLY, S_IRUSR, S_IWUSR);
    if(output == -1){
        err(2,"error while opening");
    }
    struct stat st;
    if(fstat(input, &st) !=0){
        err(3,"error with stat");
    }
    if(st.st_size % sizeof(uint16_t)!= 0){
        errx(4,"wrong size format");
    }
    off_t fileSize=st.st_size;
    uint16_t numbers=st.st_size / sizeof(uint16_t);
    const uint16_t maxN=65535 ;
    if(numbers >maxN){
        errx(5,"Too many numbers");
    }
    uint16_t* buf=malloc(numbers*sizeof(uint16_t));
    if(buf==NULL){
        err(6,"error with malloc");
    }
    ssize_t bytes_read=read(input,buf,fileSize);
    if(bytes_read == -1){
        err(7,"error while reading");
    }
    qsort(buf,numbers,sizeof(uint16_t),compareUint16);

    if(write(output,buf,fileSize)== -1){
        err(8,"error while writing");
    }
    close(input);
    close(output);
    free(buf);
    return 0;

}
//kato 67
/*Зад. 69 2018-SE-04 Напишете програма на C, която приема два параметъра – имена на файлове:
• примерно извикване: ./main input.bin output.bin
• файловете input.bin и output.bin се третират като двоични файлове, състоящи се от uint16_t
числа
• файлът input.bin може да съдържа максимум 65535 числа
• файлът output.bin трябва да бъде създаден от програмата и да съдържа числата от input.bin,
сортирани във възходящ ред
• endianness-ът на машината, създала файла input.bin е същият, като на текущата машина
• ограничения на ресурси: програмата трябва да работи с употреба на максимум 256 KB RAM
и 2 MB дисково пространство. */
