________________________________________
73
------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){

        if( argc != 3){
                errx(1, "wrong arg");
        }
        int scl=open(argv[1],O_RDONLY);
        if(scl <0){
                err(2,"error while opening file");
        }

        int sdl=open(argv[2],O_RDONLY);
        if(sdl < 0){
                err(2,"error while opening file");
        }
        int output=open("output.bin" , O_CREAT|O_TRUNC| O_WRONLY, S_IRWXU);
        if (output<0){
                err(2,"error while opening file");
        }
        struct stat st_scl;
        struct stat st_sdl;
        int r=0;
        r=fstat(sdl, &st_sdl);
        if( r== -1){
                err(3,"cannot stat");
        }

        r=fstat(scl, &st_scl);
        if(r == -1){
                err(3,"cannot stat");
        }

        //if(st_scl.st_size/ sizeof(uint16_t) != (long unsigned int)(st_scl.st_size * CHAR_BIT)){
        if(st_sdl.st_size/ sizeof(uint16_t) != st_scl.st_size*sizeof(uint8_t)){
                errx(6, "err with size");
        }
        uint16_t readFromSDL;
        uint8_t buf;
        uint16_t counter=0;
        ssize_t bytes_read;
        while ( (bytes_read=read(scl, &buf, sizeof(buf))) > 0){
                for(int i=7; i>=0;i--){
                        if( (buf & (1<< i)) != 1){
                                lseek(sdl, (counter)*sizeof(uint16_t), SEEK_SET);

                                if(read(sdl, &readFromSDL, sizeof(readFromSDL)) != sizeof(readFromSDL)){
                                        err(4,"error while reading");
                                }
                                if(write(output, &readFromSDL, sizeof(readFromSDL)) != sizeof(readFromSDL)){
                                        err(5,"error while writing");
                                }
                        }
                        counter++;
                }
        }
        if(bytes_read < 0 ){
                err(4,"error while reading");
        }        
        close(scl);
        close(sdl);
        close(output);


        return 0;
}


/*Зад. 73 2020-SE-02 Инженерите от съседната лабораторя работят с комплекти SCL/SDL файлове, напр.
input.scl/input.sdl.
В SCL файла са записани нива на сигнали (ниско 0 или високо 1), т.е., файлът се третира като със-
тоящ се от битове. В SDL файла са записани данни от тип uint16_t, като всеки елемент съответства
позиционно на даден бит от SCL файла.
Помогнете на колегите си, като напишете програма на C, която да бъде удобен инструмент за
изваждане в нов файл само на тези SDL елементи, които са имали високо ниво в SCL файла,
запазвайки наредбата им. */
______________________________________________
73- niq

#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[]){
        if(argc != 3){
                errx(1, "We need 2 arguments");
        }

        struct stat st;
        if(stat(argv[2], &st) < 0){
                err(2, "Error stat");
        }

        if(st.st_size % 16 != 0){
                errx(3, "Wrong format file");
        }

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

        uint16_t input[8];

        int fd1 = open(argv[1], O_RDONLY);
        if(fd1 < 0){
                err(4, "Error opening");
        }

        int fd2 = open(argv[2], O_RDONLY);
        if(fd2 < 0){
                err(4, "Error opening");
        }

        int fd3 = open("file.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
        if(fd3 < 0){
                err(4, "Error opening");
        }

        uint8_t bufBit;

        //proverki trqbva da ima
        while(read(fd1, &bufBit, sizeof(bufBit)) > 0){

                read(fd2, &input, sizeof(input));
                for(int i = 7; i >= 0; i--){
                        if((bufBit & one[i]) != 0){
                                write(fd3, &input[7-i], sizeof(uint16_t));
                        }
                }
        }


        exit(0);
}
