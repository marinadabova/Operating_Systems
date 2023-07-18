
#include <err.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        errx(1, "We need 2 arguments");
    }

    if((strcmp(argv[1], "-d") != 0) && (strcmp(argv[1], "-s") != 0)){
        if(strlen(argv[1]) != strlen(argv[2])){
            errx(2, "The first and second must have same length");
        }
    }

    if(strcmp(argv[1],"-d")==0){
        if(argc != 3){
            errx(1,"We need 2 arguments");
        }
        char* set1=argv[2];
        char buf;
        bool print=true;
        ssize_t bytes_read;
        while( (bytes_read=read(0,&buf,sizeof(buf)))> 0){
                for(size_t i=0;i<strlen(argv[2]);i++){
                    if(buf== set1[i]){
                        print= false;
                    }
                }
                if(print){
                    if(write(1,&buf,bytes_read)!= bytes_read){
                        err(4,"Error while writing");
                    }
                }
                print=true;
        }
        if(bytes_read== -1){
            err(3,"Error while reading");
        }
    }
    else if(strcmp(argv[1],"-s")== 0){
        if(argc != 3){
            errx(1,"We need 2 arguments");
        }
        char prev=' ';
        char* set1=argv[2];
        char buf;
        ssize_t bytes_read;
        while( (bytes_read=read(0,&buf,sizeof(buf)))> 0){
                if((buf != prev &&buf == set1[0])|| (buf != set1[0])){
                    if(write(1,&buf,bytes_read)!= bytes_read){
                        err(4,"Error while writing");
                    }
                    prev=buf;
                }
        }
        if(bytes_read== -1){
            err(3,"Error while reading");
        }
    }
    else{
        char* set1 = argv[1];
        char* set2 = argv[2];
        char buf;
        ssize_t bytes_read;

        while( (bytes_read=read(0,&buf,sizeof(buf)))> 0){
            for(size_t i=0;i<strlen(argv[2]);i++){
                if(buf == set1[i]){
                    buf=set2[i];
                }
            }
            if(write(1,&buf,bytes_read)!= bytes_read){
                err(4,"Error while writing");
            }
        }
        if(bytes_read== -1){
            err(3,"Error while reading");
        }
    }
    return 0;
}



/*Зад. 66 2018-SE-01 Напишете програма на C, която да работи подобно на командата tr, реализирайки
само следната функционалност:
• програмата чете от стандартния вход и пише на стандартния изход
34
• общ вид на изпълнение: ./main [OPTION] SET1 [SET2]
• OPTION би могъл да бъде или -d, или -s, или да липсва
• SET1 и SET2 са низове, в които знаците нямат специално значение, т.е. всеки знак “означава”
съответния знак. SET2, ако е необходим, трябва да е същата дължина като SET1
• ако е подаден като първи параметър -d, програмата трие от входа всяко срещане на знак μ
от SET1; SET2 не е необходим
• ако е подаден като първи параметър -s, програмата заменя от входа всяка поредица от повто-
рения знак μ от SET1 с еднократен знак μ; SET2 не е необходим
• в останалите случаи програмата заменя от входа всеки знак μ от SET1 със съответстващият
му позиционно знак ν от SET2.
Примерно извикване:
$ echo asdf | ./main -d ’d123’ | ./main ’sm’ ’fa’ | ./main -s ’f’
af*/
