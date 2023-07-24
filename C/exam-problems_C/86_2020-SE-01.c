#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

struct str{

        char filename[8];
        uint32_t offset;
        uint32_t length;
};
int main (int argc, char* argv[]){
        if (argc != 2){
                errx(1,"One argument needed");
        }
        int fd = open(argv[1], O_RDONLY);

        if (fd == -1){
                err(2,"Failed opening");
        }
        //proverka na structurata - sus stat
        struct str s; //-tova e header
        struct str arr[8];

        struct stat st;
        if (fstat(fd, &st) == -1){
                err(3,"Failed stat");

        }
        if (st.st_size % sizeof(s) != 0 || st.st_size > 8*sizeof(s) ){
                errx(4,"Wrong file format");
        }
        int count = st.st_size/sizeof(s);
        if (read(fd, arr, count*sizeof(s)) != count*sizeof(s) ){
                err(5,"Could not read");
        }
        close(fd);


        for(int i=0; i<count; i++){

                if(arr[i].filename[7] != "0x00"){
                        errx(6,"Wrong file format");
                }


        }
        int pd[2];
        if (pipe(pd) == -1){
                err(7,"Could not pipe");
        }

        int i = 0;
        int f;
        for(; i<count; i++){
                f = fork();

                if(f == -1){
                        err(8,"could not fork");

                }
                else if( f == 0){
                        close(pd[0]);
                        break;

                }

        }
        uint16_t r = 0x0000;
        if( f == 0){
                int fd1 = open(arr[i].filename, O_RDONLY);
                if(fd1 == -1){
                        err(9,"could not open fd1");
                }
                if ( lseek(fd1, arr[i].offset*sizeof(uint16_t), SEEK_SET) == -1){
                        err(10,"problem with lseek");
                }

//      }

        uint16_t buf;

        for (uint32_t j=0; j<arr[i].length; j++){
                if (read(fd1, &buf, sizeof(buf)) == -1){

                        err(11,"could not read of buf");
                }
                r = r ^ buf; // r^=buf


                }
                if (write(pd[1], &r, sizeof(r)) == -1){
                        err(23,"Could not write");
                }
                close(pd[1]);
                exit(0);
        }
        uint16_t rr = 0x0000;
        for(int j=0; j<count; j++){
                wait(NULL);
                uint16_t buf;
                close(pd[1]);
                if (read(pd[0], &buf, sizeof(buf)) == -1){
                        err(24,"could not read");
                }

                rr = rr ^ buf;

        }
        close(pd[0]);
        dprintf(1,"04%X\n",r);
        exit(0);
}
/* Зад. 86 2020-SE-01 Напишете две програми на C (foo и bar), които си комуникират през наименована
тръба. Програмата foo приема параметър - име на файл, програмата bar приема параметър -
команда като абсолютен път до изпълним файл.
Примерни извиквания и ред на изпълнение (в отделни терминали):
./foo a.txt
./bar /usr/bin/sort
Програмата foo трябва да изпълнява външна команда cat с аргумент името на подадения файл,
така че съдържанието му да се прехвърли през тръбата към програмата bar, която от своя страна
трябва да изпълни подадената и като аргумент команда (без параметри; /usr/bin/sort в примера),
която да обработи получените през тръбата данни, четейки от стандартен вход. Еквивалент на
горния пример би било следното изпълнение:
cat a.txt | /usr/bin/sort*/
