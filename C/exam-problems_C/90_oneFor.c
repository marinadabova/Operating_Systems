
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
        if(argc != 3){
                errx(1,"wrong number of arguments");
        }
        if (strlen(argv[1]) > 1){
                errx(2,"wrong format");
        }
        if(strlen(argv[2]) >1 ){
                errx(2,"wrong format");

        }
        int N=atoi(argv[1]);
        int D=atoi(argv[2]);

        if(N <0 || N>9){
                errx(2,"wrong format");

        }
        if(D< 0 || D>9){
                errx(2,"wrong format");

        }

        int pd1[2];
        int pd2[2];
        if(pipe(pd1) == -1){
                err(3, "error with pipe");
        }
        if(pipe(pd2) == -1){
                err(3, "error with pipe");
        }
        pid_t p=fork();
        if(p <0){
                err(4,"error with fork");
        }
        if(p == 0){
                close(pd1[1]);
                close(pd2[0]);
        }
        else{
                close(pd1[0]);
                close(pd2[1]);
        }

        for(int i=0;i<N;i++){
                if(p ==0){
                        char buf;
                        if(read(pd1[0],&buf,1) <0){
                                err(6,"error with read");
                        }
                        if(write(1,"DONG\n",5) <0){
                                err(5,"error with write");
                        }
                        if(write(pd2[1],"D",1) <0){
                                err(5,"error with write");
                        }
                }
                else{
                        if(write(1,"DING ",5) <0){
                                err(5,"error with write");
                        }
                        if(write(pd1[1],"D", 1) <0 ){
                                err(5,"error with write");
                        }
                        char buf;
                        if( read(pd2[0],&buf,1) <0){
                                err(6,"error with read");
                        }
                        sleep(D);
                }
        }
        if(p ==0){
                close(pd1[0]);
                close(pd2[1]);
        }
        else{
                close(pd1[1]);
                close(pd2[0]);
        }

        return 0;
}
