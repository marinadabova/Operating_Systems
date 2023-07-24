
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// cut -d ':' -f 7 /etc/passwd | sort | uniq -c | sort -n -k 1,1
//                          ------>  ------>  ----->
//                        a[1] a[0] b[1] b[0] c[1] c[0]
int main(void){
        int a[2];
        if(pipe(a) < 0){
                err(1,"error with pipe");
        }
        pid_t p=fork();
        if(p <0 ){
                err(2,"error with fork");
        }
        else if( p == 0){
                close(a[0]);
                if(dup2(a[1],1) <0){
                        err(3,"error with dup2");
                }
                if(execlp("cut","cut", "-d:", "-f7","/etc/passwd", (char*)NULL) <0){
                        err(4,"error while execlp");
                }
        }
        close(a[1]);
        int b[2];
        if(pipe(b) <0){
                err(1,"error with pipe");
        }
        pid_t q=fork();
        if(q<0){
                err(2,"error with fork");
        }
        else if(q==0){
                close(b[0]);
                if(dup2(a[0],0) <0){
                        err(3,"error with dup2");
                }
                if(dup2(b[1],1)< 0){
                        err(3,"error with dup2");
                }
                if( execlp("sort", "sort", (char*) NULL) < 0){
                        err(4,"error while execlp");
                }
        }
        close(b[1]);
        close(a[0]);
        int c[2];
        if(pipe(c) < 0){
                err(1,"error with pipe");
        }
        pid_t r=fork();
        if(r <0){
                err(2,"error with fork");
        }
        else if( r== 0){
                close(c[0]);
                if (dup2(b[0],0) <0){
                        err(3,"error with dup2");
                }
                if(dup2(c[1],1) <0){
                        err(3,"error with dup2");
                }
                if(execlp("uniq", "uniq", "-c", (char*) NULL) <0 ){
                        err(4,"error while execlp");
                }
        }
        close(c[1]);
        close(b[0]);

        if(dup2(c[0],0) <0){
                err(3,"error with dup2");
        }
        if(execlp("sort", "sort", "-n", "-k1,1", (char*)NULL) < 0){
                err(4,"error while execlp");
        }

        exit(0);
}

/*Зад. 82 2017-IN-01 Напишете програма на C, която използвайки външни shell команди през pipe() да
извежда статистика за броя на използване на различните shell-ове от потребителите, дефинирани
в системата. Изходът да бъде сортиран във възходящ ред според брой използвания на shell-овете.
Примерно извикване и изход:
$ ./main
1 /bin/sync
3 /bin/bash
7 /bin/false
17 /usr/sbin/nologin */
