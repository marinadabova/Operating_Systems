#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct data {
    uint16_t header;
    uint16_t count;
    uint16_t other;
} __attribute__((packed)) Data;

typedef struct secondData {
    uint16_t header;
    uint16_t count;
    uint8_t other;
} __attribute__((packed)) SecondData;

typedef struct thirdData {
    uint16_t header;
    uint16_t count;
    uint32_t other;
} __attribute__((packed)) ThirdData;

typedef struct fourthData {
    uint16_t header;
    uint16_t count;
    uint64_t other;
} __attribute__((packed)) FourthData;

int main(int argc, char* argv[]){
    if(argc != 7){
        errx(1, "Need 6 arguments");
    }
    if (argc != 7) {
        errx(1, "Need 6 arguments");
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        err(2, "Cannot open");
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1) {
        err(2, "Cannot open");
    }

    int fd3 = open(argv[3], O_RDONLY);
    if (fd3 == -1) {
        err(2, "Cannot open");
    }

    int fd4 = open(argv[4], O_RDONLY);
    if (fd4 == -1) {
        err(2, "Cannot open");
    }

    int fd5 = open(argv[5], O_RDONLY);
    if (fd5 == -1) {
        err(2, "Cannot open");
    }

    int fd6 = open(argv[6], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd6 == -1) {
        err(2, "Cannot open");
    }

    Data affix;
    ThirdData postfix;
    SecondData prefix;
    Data infix;
    FourthData suffix;
    SecondData crucifixus;


    uint16_t buf16;
    uint8_t buf8;
    uint32_t buf32;
    uint64_t buf64;
// checks
     struct stat st1, st2, st3, st4, st5, st6;
    if (fstat(fd1, &st1) != 0 || fstat(fd2, &st2) != 0 || fstat(fd3, &st3) != 0 ||
        fstat(fd4, &st4) != 0 || fstat(fd5, &st5) != 0 || fstat(fd6, &st6) != 0) {
        err(3, "Error with fstat");
    }

    if (st1.st_size % sizeof(Data) != 0 || st2.st_size % sizeof(SecondData) != 0 ||
        st3.st_size % sizeof(Data) != 0 || st4.st_size % sizeof(FourthData) != 0 ||
        st5.st_size % sizeof(ThirdData) != 0 || st6.st_size % sizeof(Data) != 0) {
        errx(4, "Error with file format");
    }

    if (st1.st_size / sizeof(Data) != affix.count || st2.st_size / sizeof(SecondData) != prefix.count ||
        st3.st_size / sizeof(Data) != infix.count || st4.st_size / sizeof(FourthData) != suffix.count ||
        st5.st_size / sizeof(ThirdData) != postfix.count || st6.st_size / sizeof(Data) != 0) {
        errx(4, "Error with count and the number of nums");
    }

    ssize_t read_bytes;
    uint16_t interval_start = 0;
    uint16_t interval_count = 0;

    while( (read_bytes=read(fd1,&buf16,sizeof(uint16_t)))>0){
        if (lseek(fd1, (off_t)(affix.count * sizeof(uint16_t)), SEEK_CUR) == -1)
        {
            err(5, "Error with lseek");
        }

        if (interval_count == 0)
        {
            interval_start = buf16;
        }
        else
        {
            for (uint16_t i = interval_start; i < buf16; ++i)
            {
                crucifixus.other = prefix.other;
                if (write(fd6, &crucifixus, sizeof(SecondData)) != sizeof(SecondData))
                {
                    err(6, "Error writing");
                }
            }
            interval_start = buf16;
        }

        ++interval_count;
    }

    if (read_bytes == -1)
    {
        err(7, "Error with reading");
    }

    for (uint16_t i = interval_start; i < affix.count; i++) {
        crucifixus.other = prefix.other;
        if (write(fd6, &crucifixus, sizeof(SecondData)) != sizeof(SecondData))
        {
            err(6, "Error writing");
        }
    }

    while (read(fd5, &postfix, sizeof(ThirdData)) == sizeof(ThirdData))
    {
        for (uint16_t i = 0; i < postfix.count; ++i)
        {
            if (write(fd6, &postfix, sizeof(ThirdData)) != sizeof(ThirdData))
            {
                err(6, "Error writing");
            }
        }
    }

    while (read(fd3, &infix, sizeof(Data)) == sizeof(Data))
    {
        for (uint16_t i = 0; i < infix.count; ++i)
        {
            if (write(fd6, &infix, sizeof(Data)) != sizeof(Data))
            {
                err(6, "Error writing");
            }
        }
    }

    while (read(fd4, &suffix, sizeof(FourthData)) == sizeof(FourthData))
    {
        for (uint16_t i = 0; i < suffix.count; ++i)
        {
            if (write(fd6, &suffix, sizeof(FourthData)) != sizeof(FourthData))
            {
                err(6, "Error writing");
            }
        }
    }


    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);
    return 0;
}
/*Зад. 72 2020-SE-01 Напишете програма на C, която приема шест задължителни позиционни параметъра
– имена на файлове. Примерно извикване:
$ ./main affix postfix prefix infix suffix crucifixus
Всички файлове започват с хедър с фиксирана дължина от 16 байта. Пети и шести (спрямо Z+)
байт от хедъра дефинират uint16_t число count, което описва броя на елементите във файла.
Файловете affix и infix се състоят от елементи от тип uint16_t, файловете prefix и crucifixus
– от елементи от тип uint8_t, postfix – от uint32_t, а suffix – от uint64_t.
Интервал наричаме наредена двойка числa, която дефинира номер (спрямо Z) на начален еле-
мент и брой елементи от даден файл. Комплект наричаме наредена четворка от интервали, които
позиционно се отнасят спрямо файловете {post,pre,in,suf}fix.
Елементите на файлa affix дефинират серия от комплекти, на чиято база програмата трябва да
генерира изходния файл crucifixus. */
