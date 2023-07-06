#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){

        if(argc == 1){ //nqma param
                char buf[4096];
                ssize_t bytes_read;

                while( (bytes_read=read(0,&buf,sizeof(buf))) > 0){
                        if( write(1, &buf, bytes_read) != bytes_read) {
                                err(2,"err while writing to stdout");
                        }
                }
                if(bytes_read < 0){
                        err(1, "err while reading");
                }
                exit(0);
        }

        for( int i=1;i< argc; i++){
                int fd;
                ssize_t bytes_read;
                if(strcmp(argv[i], "-") == 0){
                        fd=0;
                }
                else{
                        fd=open(argv[i],O_RDONLY);
                        if(fd < 0){
                                 warn("Cant open file"); // koe trqbva da e?
                                //err(3,"error opening");
                                continue;
                        }
                }
                char buf[4096];
                while( (bytes_read= read(fd, &buf, sizeof(buf))) >0 ){
                        if( write(1, &buf, bytes_read) != bytes_read){
                                err(2,"err while writing to stdout");
                        }
                }
                if( bytes_read == -1){ // <0 ?
                        err(1,"err while reading");
                }
                if(fd != 0){
                        close(fd);
                }
        }



        return 0;
}

/*Зад. 65 2017-SE-04 Напишете програма на C, която да работи подобно на командата cat, реализирайки
само следната функционалност:
• програмата извежда на STDOUT
• ако няма подадени параметри, програмата чете от STDIN
• ако има подадени параметри – файлове, програмата последователно ги извежда
• ако някой от параметрите започва с тире (-), програмата да го третира като специално име
за STDIN
Примерно извикване:
$ ./main f - g
– извежда съдържанието на файла f, после STDIN, след това съдържанието на файла g. */
