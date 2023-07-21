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
------------------------------------------------------------------------------------
        // Зад. 72 2021-SE-01 Инженерите от съседната лабораторя ползват специализиран хардуер и софтуер за прехвърляне на данни по радио, но за съжаление имат два проблема:
// • в радио частта: дълги поредици битове само 0 или само 1 чупят преноса;
// • в софтуерната част: софтуерът, който ползват, може да прехвърля само файлове с четен брой байтове дължина.
// Помогнете на колегите си, като напишете програма на C, която решава тези проблеми, като подготвя файлове за прехвърляне. Програмата трябва да приема два задължителни позиционни аргумента – имена на файлове. 
// Примерно извикване: $ ./main input.bin output.bin
// Програмата чете данни от input.bin и записва резултат след обработка в output.bin. Програмата трябва да работи като encoder, който имплементира вариант на Manchester code, т.е.:
// • за всеки входен бит 1 извежда битовете 10, и
// • за всеки входен бит 0 извежда битовете 01
// Например, следните 8 бита вход
// 1011 0110 == 0xB6
// по описаният алгоритъм дават следните 16 бита изход
// 1001 1010 0110 1001 == 0x9A69

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <err.h>

bool get_bit(uint8_t byte, int idx)
{
    return ((byte & (1 << idx)) != 0);
}

uint16_t set_bit(uint16_t x, int idx, bool bit)
{
    if(bit)
    {
        return (1 << idx) | x ;
    }
    else
    {
        return (~(1 << idx)) & x ;
    }
}

uint16_t manchester_encode_byte(uint8_t byte)
{
    uint16_t result = 0;
    for (int i = 0; i < 8; ++i)
    {
        bool bit = get_bit(byte, i);
        if (bit)
        {
            result = set_bit(result, i * 2, 1);
            result = set_bit(result, i * 2 + 1, 0);
        }
        else
        {
            result = set_bit(result, i * 2, 0);
            result = set_bit(result, i * 2 + 1, 1);
        }
    }

    return result;
}

void manchester_encode_bytes(uint8_t* in_buf, uint16_t* out_buf, int n)
{
    for(int i = 0; i < n; ++i)
    {
        out_buf[i] = manchester_encode_byte(in_buf[i]);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        warnx("%s encodes a file to manchester encoding", argv[0]);
        errx(1, "usage: %s <input file> <output file>", argv[0]);
    }
    
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0)
    {
        err(1, "Could not open %s", argv[1]);
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_out < 0)
    {
        err(1, "Could not open %s", argv[1]);
    }

    uint8_t in_buf[4096];
    uint16_t out_buf[4096];

    int bytes_read;
    while((bytes_read = read(fd_in, in_buf, sizeof(in_buf))) > 0)
    {
        manchester_encode_bytes(in_buf, out_buf, bytes_read * 2);
        int bytes_written = write(fd_out, &out_buf, bytes_read * 2);
        if (bytes_written < 0)
        {
            err(1, "Could not write data to %s", argv[2]);
        }
        if (bytes_read != bytes_written)
        {
            errx(1, "Could not write all data at once");
        }
    }

    if (bytes_read < 0)
    {
        err(1, "Could not read data from %s", argv[1]);
    }
    
    close(fd_in);
    close(fd_out);
    return 0;
}
