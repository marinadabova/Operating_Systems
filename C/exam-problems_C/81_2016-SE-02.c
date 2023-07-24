
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(void){
        while(true){
                char buf[1024];
                ssize_t bytes_read;
                /* if ((write(1, "Enter command: ", 15)) != 15)
        {
            err(1, "Write failed!");
        }
                */
                if( (bytes_read=read(0,&buf,sizeof(buf))) < 0){
                        err(1,"error while reading");
                }
                buf[bytes_read - 1] ='\0';
                if(strcmp(buf,"exit") == 0){
                        break;
                }
                pid_t p=fork();
                if(p<0){
                        err(2,"error fork");
                }
                else if(p == 0){
                        if(execlp(buf, buf, (char*)NULL) == -1){
                                err(3,"error with execlp");
                        }
                }
                /*
        int status;
        if (wait(&status) == -1)
        {
            err(4, "Could not wait for child!");
        }

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) != 0)
            {
                errx(4, "Wait failed!");
            }
        } */

        wait(NULL);
    }
    exit(0);
}

/*Зад. 81 2016-SE-02 Напишете програма на C, която реализира simple command prompt. Тя изпълнява в
цикъл следната поредица действия:
1. Извежда промпт на стандартния изход.
2. Прочита име на команда.
3. Изпълнява без параметри прочетената команда.
Командите се търсят в директорията /bin. За край на програмата се смята въвеждането на exit. */
