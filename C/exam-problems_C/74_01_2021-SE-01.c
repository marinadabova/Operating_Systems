#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

uint16_t calc(uint8_t);
int main(int argc, char* argv[]){

        if(argc != 3){
                errx(1,"Wrong number of arguments");
        }

        int fd1=open(argv[1], O_RDONLY);
        if(fd1 < 0){
                err(2,"error while opening file");
        }
        int fd2=open(argv[2], O_CREAT| O_TRUNC| O_RDWR, S_IRUSR| S_IWUSR);
        if (fd2 < 0){
                err(2,"error while opening file");
        }
        struct stat st1;
        struct stat st2;

        if( stat(argv[1], &st1) < 0){
                err(3,"err with stat");
        }
        if(stat(argv[2], &st2) < 0){
                err(3,"err with stat");
        }
        //if( st2.st_size /sizeof(uint16_t)??
        uint16_t encode;
        uint8_t buf;
        ssize_t bytes_read;

        while( (bytes_read=read(fd1, &buf, sizeof(uint8_t))) > 0){// zashto ne e sizeof(buf)
                encode=calc(buf);// ne go shvashtam

                if(write(fd2, &encode, sizeof(uint16_t)) != sizeof(uint16_t)){
                        err(5,"error while writing");
                }
        }
        if(bytes_read < 0){
                err(4,"error while reading");
        }

        close(fd1);
        close(fd2);
        return 0;
}
uint16_t calc(uint8_t x){
        uint8_t one[8] = {
                    0x01,
                0x02,
                0x04,
                0x08,
                0x10,
                0x20,
                0x40,
                0x80,
        };

        uint16_t two[8] = {
                0x0002,
            0x0008,
                0x0020,
            0x0080,
                0x0200,
            0x0800,
            0x2000,
            0x8000,
        };

        uint16_t three[8] = {
                0x0001,
            0x0004,
                0x0010,
            0x0040,
                0x0100,
            0x0400,
            0x1000,
            0x4000,
        };

        uint16_t result = 0;
        for(int i = 0; i < 8; i++){
                if((x & one[i]) != 0){
                        result = result | two[i];
                }
                else{
                        result = result | three[i];
                }
        }

        return result;
}
/*Зад. 74 2021-SE-01 Инженерите от съседната лабораторя ползват специализиран хардуер и софтуер за
прехвърляне на данни по радио, но за съжаление имат два проблема:
• в радио частта: дълги поредици битове само 0 или само 1 чупят преноса;
• в софтуерната част: софтуерът, който ползват, може да прехвърля само файлове с четен брой
байтове дължина.
Помогнете на колегите си, като напишете програма на C, която решава тези проблеми, като подготвя
файлове за прехвърляне. Програмата трябва да приема два задължителни позиционни аргумента
– имена на файлове. Примерно извикване:
$ ./main input.bin output.bin
Програмата чете данни от input.bin и записва резултат след обработка в output.bin. Програмата
трябва да работи като encoder, който имплементира вариант на Manchester code, т.е.:
• за всеки входен бит 1 извежда битовете 10, и
• за всеки входен бит 0 извежда битовете 01
Например, следните 8 бита вход
1011 0110 == 0xB6
по описаният алгоритъм дават следните 16 бита изход
1001 1010 0110 1001 == 0x9A69 */
