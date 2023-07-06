#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
        if(argc != 4){
                errx(1,"wrong number of arguments");
        }
        struct data{
                uint16_t ofset;
        uint8_t b1;
        uint8_t b2;
    }__attribute__((packed));

        struct data patch;

        struct stat st1;
        if(stat(argv[1], &st1) < 0){
                err(3,"error stat");
        }
        if (st1.st_size % sizeof(patch) != 0){
                 err(4,"patch file not in right format");
        }
        struct stat st2;

        if(stat(argv[2], &st2) < 0){
                 err(3,"error stat");
        }
        if(st2.st_size > 0xFFFF){
                warnx("Too big file. Possible mistakes");
    }

        int p=open(argv[1], O_RDONLY);
        if(p < 0){
                err(2,"error while opening");
        }
        int fd1=open(argv[2], O_RDONLY);
        if (fd1 < 0){
                err(2,"error while opening");
        }

        int fd2=open(argv[3],O_CREAT| O_TRUNC, O_RDWR, S_IRUSR, S_IWUSR);
        if (fd2 < 0){
                err(2,"error while opening");
        }

        uint8_t buf;
        ssize_t bytes_read;

        while( (bytes_read=read(fd1, &buf, sizeof(buf))) >0){
                if(write(fd2, &buf, bytes_read)< 0){
                        err(5,"error while writing");
                }
        }

        if (bytes_read == -1){
                err(6,"error while reading");
        }

        while( (bytes_read=read(p, &patch, sizeof(patch))) > 0){
                if(patch.ofset>st2.st_size){
                        errx(7,"incorrect offset out of file");
                }

                if(lseek(fd2, patch.ofset,SEEK_SET) <0){
                        err(8,"error lseek");
                }
                if(read(fd2, &buf, sizeof(buf)) <0){
                        err(6,"error while reading");
                }
                if (buf != patch.b1){
                        errx(9,"different pathc file");
                }
                if(lseek(fd2, patch.ofset, SEEK_SET) < 0){
            err(8, "Error lseek");
        }

       if(write(fd2, &patch.b2, sizeof(patch.b2)) < 0){
                err(6, "Error writnig");
       }
    }
    if(bytes_read < 0){
        err(6,"error while reading");
    }
        close(p);
        close(fd1);
        close(fd2);
        return 0;
}

/*Зад. 64 2017-SE-03 Напишете програма на C, която приема три параметъра, имена на двоични файлове.
Примерно извикване:
$ ./main patch.bin f1.bin f2.bin
Файловете f1.bin и f2.bin се третират като двоични файлове, състоящи се от байтове (uint8_t).
Файлът patch.bin е двоичен файл, състоящ се от наредени тройки от следните елементи (и техните
типове):
• отместване uint16_t
• оригинален байт uint8_t
• нов байт uint8_t
Програмата да създава файла f2.bin като копие на файла f1.bin, но с отразени промени на базата
на файла patch.bin, при следния алгоритъм:
• за всяка наредена тройка от patch.bin, ако на съответното отместване (в байтове) спрямо
началото на файла е записан байта оригинален байт, в изходния файл се записва нов байт.
Ако не е записан такъв оригинален байт или такова отместване не съществува, програмата
да прекратява изпълнението си по подходящ начин;
• всички останали байтове се копират директно.
Забележка: Наредените тройки във файла patch.bin да се обработват последователно.
Примерен f1.bin:
00000000: f5c4 b159 cc80 e2ef c1c7 c99a 2fb0 0d8c ...Y......../...
00000010: 3c83 6fed 6b46 09d2 90df cf1e 9a3c 1f05 <.o.kF.......<..
00000020: 05f9 4c29 fd58 a5f1 cb7b c9d0 b234 2398 ..L).X...{...4#.
00000030: 35af 6be6 5a71 b23a 0e8d 08de def2 214c 5.k.Zq.:......!L
Примерен patch.bin:
00000000: 0200 b159 3000 35af ...Y0.5.
Примерен f2.bin:
00000000: f5c4 5959 cc80 e2ef c1c7 c99a 2fb0 0d8c ..YY......../...
00000010: 3c83 6fed 6b46 09d2 90df cf1e 9a3c 1f05 <.o.kF.......<..
00000020: 05f9 4c29 fd58 a5f1 cb7b c9d0 b234 2398 ..L).X...{...4#.
00000030: afaf 6be6 5a71 b23a 0e8d 08de def2 214c ..k.Zq.:......!L */
