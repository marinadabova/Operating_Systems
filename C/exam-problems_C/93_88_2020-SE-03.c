#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define N 8
struct data{
        char name[8]; //8 bytes
        uint32_t offset; //4 bytes
        uint32_t length; //4bytes
}__attribute__((packed));

int main(int argc, char* argv[]){
    if( argc != 2){
        errx(1,"wrong number of arguments");
    }
    struct data d;
    struct stat st; //obsto e 16 bytes
    if(stat(argv[1],&st) <0){
        err(2,"error while stat");
    }
    if(st.st_size % sizeof(d) != 0 || (long unsigned int)st.st_size > 8*sizeof(d)){
        // tuk trqbva da proverq dali razmera na faile e kraten na razmera na struct(16bytes obshto e struct)
        // i sled tova proverqvam dali razmera na file e po-golqm ot 8* 16 (128bytes) - 8 puti zahstoot iskam ne poveche ot 8 naredeni 3ki(t.e structuri)
        errx(3,"wrong format");
    }
    int fd=open(argv[1], O_RDONLY);
    if( fd == -1){
        err(4,"error while opening1");
    }

    int countOfStr= st.st_size /sizeof(d); //vzimam si broq naredeni 3ki ot fila koito sme podali kato si razdelq na size-a na struct d koeto e 16 bytes

    if(countOfStr > N){
        errx(5,"wrong format file");
    }

    struct data arr[8];

    int p[2];
    if(pipe(p)< 0){
        err(6,"error with pipe");
    }

    pid_t x;
    int i;

    for(i=0; i<countOfStr; i++){ // za vsqka str (nar 3ka) iskam da si suzdam child process
        if(read(fd, &arr[i], sizeof(d)) != sizeof(d)){ //cheta si vsqka structurka arr[i]
                err(7,"error while reading");
        }
        x=fork();
        if( x== -1){
                err(8,"error with fork");
        }
        if (x==0){ // kato e v child-a break-vam, no parenta produljava da iterira v cikula i da suzdava novi child processi- suzdavat se paralelno kakto se iska v zad
                break;
        }
        }

    if(x==0){  //ako sme v deteto
        close(p[0]);

        uint16_t result_child=0;  //shte si pazq resulta ot xor-vaneto na vs element na vseki child
        uint16_t buf=0; // elementite vuv vs fail (uint16_t)

        int curfd=open(arr[i].name, O_RDONLY);
        if(curfd == -1){
                err(4,"error while opening");
        }
        struct stat st1;

        if(stat(arr[i].name , &st1) < 0){
                err(2,"error while stat");
        }

        if(st1.st_size % sizeof(uint16_t) !=0){ //proverqvam dali size na file-a e kraten na uint16_t -po uslovie imame che e ot uint16_t elementa
                errx(3,"wrong format");
        }
        if(lseek(curfd,arr[i].offset *sizeof(uint16_t), SEEK_SET) <0){
                // offseta mi vuv vsqka struct v arr[i] e uint32_t
                //az iskam da skipna s lseek opredelen br uint16_t elementi v podadeniq curfd file
                // i zatova convertvam dad offset (uint32_t) v uint16_t kato umnojavam *sizeof(uint16_t)
                /*by multiplying arr[i].offset (given in uint32_t elements) by sizeof(uint16_t), you calculate the number of bytes needed to skip within the file to position the file pointer at the desired uint16_t element.  */
                err(9,"error with lseek");
        }

         for(uint32_t j=0; j< arr[i].length ;j++){// length- e broq elementi vuv vs file v struct

                        if(read(curfd, &buf, sizeof(buf)) != sizeof(buf)){  //cheta si vs element ot file- a
                                err(7,"error while reading");
                        }

                        result_child=result_child^buf;
                }

                if(write (p[1],&result_child, sizeof(result_child)) != sizeof(result_child)){
                        //v pipe-a si zapisvam result
                        err(10,"error while writing to pipe");
                }
                exit(0);// terminira child process-a i pokazva na parent-a che moje da produlji gore s iteraciqta na cikula i da si pusne nov child process

        }
        close(p[1]);

        uint16_t result=0; //krainiq result
        uint16_t buff;

        for(int j=0; j<countOfStr; j++){
                if(read(p[0], &buff, sizeof(buff)) != sizeof(buff)) {
                        //cheta si ot trubata
                        err(7,"error while reading");
                }

                result=result ^ buff; // xor-va vseki poluchen rezultat koito sme zapisali v trubata
        }
        // No need for wait(NULL) here to keep child processes parallel
        printf("result: %dB\n", result);

    close(fd);

        exit(0);
}
