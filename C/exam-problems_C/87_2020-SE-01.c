//moeto si resh:
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdint.h>

int main(int argc,char* argv[]){
        if(argc != 3){
                errx(1,"We need 2 argumenst");
        }
        int a[2];
        if(pipe(a) == -1){
                err(2,"Error with pipe");
        }
        pid_t p=fork();
        if (p <0 ){
                err(3,"Error with fork");
        }
        if (p == 0){
                close(a[0]);
                if(dup2(a[1],1) == -1){
                        err(4,"Error while dup2");
                }
                if(execlp("cat","cat",argv[1],(char*) NULL)== -1){
                        err(5,"Error while execlp");
                }
        }
        close(a[1]);

        int fd=open(argv[2],O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
        if(fd == -1){
                err(6,"Error while opening file");
        }

        ssize_t r_bytes;
        uint8_t b;

        while( (r_bytes= read(a[0],&b,sizeof(b)))>0){
                if(b == 0x55){
                        continue;
                }
                else if( b== 0x7D){
                        if(read(a[0],&b,sizeof(b)) <0){
                                err(7,"Error while reading form pipe");
                        }
                        uint8_t originalB=b^0x20;
                        if(originalB != 0x00 && originalB != 0x55 && originalB != 0xFF && originalB != 0x7D){
                                errx(8,"Invalid file content");
                        }
                        if(write(fd,&originalB,sizeof(originalB)) != sizeof(originalB)){
                                err(9,"Error while writing to file");
                        }
                }
                 else{ //ima li nujda
                        if(write(fd, &b, sizeof(b)) != sizeof(b)){
                err(8,"error while writing");
             }
         }

        }
        if(r_bytes <0){
                err(7,"Error while reading form pipe");
        }
        wait(NULL);
        close(a[0]);
        close(fd);

        exit(0);
}
//................................
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
        int pf[2];
        if(pipe(pf) == -1){
                err(2,"error with pipe");
        }

        pid_t p=fork();
        if(p < 0) {
                err(3,"error with fork");
        }
        else if( p == 0){
                close(pf[0]);
                if(dup2(pf[1],1) == -1){
                        err(4,"error with dup2");
                }
                if(execlp("cat","cat",argv[1],(char*) NULL) < 0){
                        err(5,"error with execlp");
                }
        }
        close(pf[1]);
        int fd=open(argv[2], O_CREAT| O_TRUNC| O_RDWR, S_IRUSR, S_IWUSR);
        if(fd < 0){
                err(6,"error while opening file");
        }
        ssize_t bytes_read;
        uint8_t buf;

        while( (bytes_read=read(pf[0],&buf, sizeof(buf))) >0){
                if(buf == 0x7D){
                        if(read(pf[0], &buf, sizeof(buf)) < 0 ){
                                err(7,"error while reading");
                        }
                        uint8_t temp=buf ^ 0x20;
                        if (temp != 0x00 && temp != 0x55 && temp != 0xFF && temp != 0x70){
                                errx(9,"wrong file format");
                        }
                        if(write(fd, &temp, sizeof(temp)) != sizeof(buf)){
                                err(8,"error while writing");
                        }
                }
                else if(buf == 0x55){
                        continue;
                }
                else{ //ima li nujda?
                        if(write(fd, &buf, sizeof(buf)) != sizeof(buf)){
                                err(8,"error while writing");
                        }

                }
        }
        if(bytes_read < 0){
                err(7,"error while reading");
        }
        close(pf[0]);
        close(fd);


        return 0;
}

/********************/
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
/*Зад. 87 2020-SE-02 При изграждане на система за пренасяне на сериен асинхронен сигнал върху ради-
опреносна мрежа се оказало, че големи поредици от битове само нули или само единици смущават
сигнала, поради нестабилно ниво. Инженерите решили проблема, като:
• в моментите, в които няма сигнал от серийният порт, вкарвали изкуствено байт 0x55 в потока;
• реалните байтове 0x00, 0xFF, 0x55 и 0x7D се кодирали посредством XOR-ване (побитова обра-
ботка с изключващо-или) с 0x20, като полученият байт се изпращал през потока, предхождан
от 0x7D, който играе ролята на escape character.
Разполагате със запис от такъв поток. Напишете програма на C, която приема два параметъра -
имена на файлове. Примерно извикване:
$ ./main input.lfld output.bin
Програмата трябва да обработва записа и да генерира output.bin, който да съдържа оригинал-
ните данни. Четенето на входните данни трябва да става посредством изпълнение на външна shell
команда. */
