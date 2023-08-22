#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

// find argv[1] -type f -printf "%f %T@\n" | sort -nr -t ' ' -k2 | cut -d ' ' -f 1 | head -n 1
//                                   a[1] a[0] b[1] b[0] c[1] c[0]

int main(int argc, char* argv[]){
    if(argc != 2){
        errx(1,"wrong number of arguments");
    }
    int a[2];
    int b[2];
    int c[2];
    if(pipe(a)== -1){
        err(2,"error with pipe");
    }
    int fd=fork();
    if(fd == -1){
        err(3,"error with fork");
    }
    if(fd == 0){
        //child
        close(a[0]);
        dup2(a[1],1);//proverka?
        if(execlp("find","find",argv[1],"-type","f", "-printf", "%f %T@\n", (char*)NULL) == -1 ){
            err(4,"error with execlp");
        }
    }
    close(a[1]);
    if(pipe(b)== -1){
        err(2,"error with pipe");
    }
    int fd2=fork();
    if(fd2 == -1){
        err(3,"error with fork");
    }
    if(fd2 == 0){
        //child
        close(b[0]);
        dup2(a[0],0); //proverka?
        dup2(b[1],1);
        if(execlp("sort", "sort", "-nr", "-k2", (char*)NULL) == -1){
            err(4,"error with execlp");
        }
    }
    close(a[0]);
    close(b[1]);

    if(pipe(c)== -1){
        err(2,"error with pipe");
    }
    int fd3=fork();
    if(fd3 == -1){
        err(3,"error with fork");
    }
    if(fd3 == 0){
        //child
        close(c[0]);
        dup2(b[0],0);//proverka?
        dup2(c[1],1);
        if(execlp("cut", "cut", "-d", " ", "-f1", (char*)NULL) == -1){
            err(4,"error with execlp");
        }
    }
    close(b[0]);
    close(c[1]);
    dup2(c[0],0);
    if(execlp("head", "head", "-n", "1", (char*)NULL) == -1){
        err(4,"error with execlp");
    }
    return 0;
}
// --------------------------------
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){
        if(argc != 2){
                errx(1,"One parameter needed");
        }
        int a[2];
        if(pipe(a) == -1){
                err(2,"error with pipe");
        }
        pid_t p=fork();
        if ( p <0){
                err(3,"error with fork");
        }
        if( p== 0){
                close(a[0]);
                if(dup2(a[1],1) == -1){
                        err(4,"error with dup2");
                }
                if(execlp("find","find",argv[1],"-type","f", "-printf", "%f %T@\n", (char*)NULL) == -1 ){
                        err(5,"error with execlp");
                }

        }
        close(a[1]);
        wait(NULL);
        int b[2];
        if(pipe(b) == -1){
                err(2,"error with pipe");
        }
        pid_t q=fork();
        if (q < 0){
                err(3,"error with fork");
        }
        if(q == 0){
                close(b[0]);
                if(dup2(a[0],0) == -1){
                        err(4,"error with dup2");
                }
                if( dup2(b[1],1) == -1){
                        err(4,"error with dup2");
                }
                if( execlp("sort","sort","-nr", "-k2", (char*)NULL) == -1){
                        err(5,"error with execlp");
                }
        }
        close(a[0]);
        close(b[1]);
        wait(NULL);
        int c[2];
        if(pipe(c) == -1){
                err(2,"error with pipe");
        }
        pid_t t=fork();
        if(t <0){
                err(3,"error with fork");
        }
        if( t== 0){
                close(c[0]);
                if(dup2(b[0],0) == -1){
                        err(4,"error with dup2");
                }
                if(dup2(c[1],1) == -1){
                        err(4,"error with dup2");
                }
                if(execlp("cut", "cut", "-d", " ", "-f1", (char*)NULL) == -1){
                        err(5,"error with execlp");
                }
        }
        close(b[0]);
        close(c[1]);
        wait(NULL);
        if(dup2(c[0],0) == -1){
                err(4,"error with dup2");
        }
        if(execlp("head", "head", "-n", "1", (char*)NULL) == -1){
                err(5,"error with execlp");
        }
        close(c[0]);
        //wait(NULL);

        exit(0);
}
