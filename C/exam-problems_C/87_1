//ot chasa
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>



int main (int argc, char* argv[]){

        if (argc != 3){
                errx(1,"Two arg needed");
        }

        int pd[2];
        if (pipe(pd) == -1){
                err(2,"error while piping");
        }
        pid_t p=fork();

        if (p == -1){
                err(3,"error during fork");
        }
        else if(p == 0){
                close(pd[0]);
                dup2(pd[1],1);
                execlp("cat", "cat", argv[1], (char*) NULL);
        }
        close(pd[1]);

        int fd = open(argv[2], O_CREAT  |O_WRONLY | O_TRUNC , S_IRWXU);
        uint8_t buf;
        while (read(pd[0], &buf, sizeof(buf)) > 0 ){
                if (buf == 0x55){
                        continue;
                }
                if (buf == 0x7D){

                        if (read(pd[0], &buf, sizeof(buf)) != sizeof(buf)){
                                err(4,"error while reading");
                        }


                        uint8_t temp = buf ^ 0x20;
                        if (temp != 0x00 && temp != 0x55 && temp != 0xFF && temp != 0x70){
                                errx(5,"wrong file format");
                        }
                        write (fd, &temp,  sizeof(temp));

                }
                else{
                        write(fd, &buf, sizeof(buf));
                }
        }
        close(fd);
        close(pd[0]);

        return 0;
}
