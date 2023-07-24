#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

typedef struct s1_header{
        uint32_t magic;
        uint32_t count;
}__attribute__((packed)) s1_header;

typedef struct s2_header{
        uint32_t magic1;
        uint16_t magic2;
        uint16_t reserved;
        uint64_t count;
}__attribute__((packed)) s2_header;

typedef struct s2_data{
        uint16_t type;
        uint16_t reserved1;
        uint16_t reserved2;
        uint16_t reserved3;
        uint32_t offset1;
        uint32_t offset2;
}__attribute__((packed)) s2_data;

int main(int argc, char* argv[]){
        if(argc != 3){
                errx(1,"wrong number of arguments");
        }
        s1_header s1_h;
        s2_header s2_h;
        s2_data s2_d;

        struct stat s1;
        struct stat s2;

        if(stat(argv[1], &s1) == -1){
                err(2,"error with stat");
        }
        if(stat(argv[2], &s2) == -1){
                err(2,"error with stat");
        }

        int fd1=open(argv[1], O_RDWR);
        if(fd1 == -1){
                err(3,"error while openning file");
        }
        int fd2=open(argv[2], O_RDONLY);
        if(fd2 == -1){
                err(3,"error while openning file");
        }
        if(read(fd1, &s1_h,sizeof(s1_h)) == -1){
                err(4,"error while reading");
        }
        if(s1_h.magic != 0x21796F4A) {
                err(5, "wrong magic number");
        }

        if(read(fd2,&s2_h, sizeof(s2_h)) == -1){
                 err(4,"error while reading");
        }
        if(s2_h.magic1 != 0xAFBC7A37){
                err(5, "wrong magic number");
        }
        if(s2_h.magic2 != 0x1C27){
                err(5, "wrong magic number");
        }
        if( (s1.st_size -sizeof(s1_h)) % s1_h.count != 0){
                err(6,"wrong size of f1");
        }

        if( (s1.st_size -sizeof(s1_h)) % sizeof(uint16_t) != 0){
                err(6,"wrong size of f1 data");
        }
        if( (s2.st_size -sizeof(s2_h)) % s2_h.count != 0){
                err(6,"wrong size of f2");
        }


        ssize_t rs=-1;
        uint64_t el1;
        uint64_t el2;

        while( (rs=read(fd2, &s2_d, sizeof(s2_d))) > 0){
                if(rs == -1){
                        err(7,"error while reading");
                }

                if(s2_d.type != 0 && s2_d.type != 1){
                        errx(8,"type should be 1 or 0");
                }
                if (s2_d.reserved1 != 0 || s2_d.reserved2 != 0 || s2_d.reserved3 != 0){
                        errx(9,"all reserved should be 0");
                }

                if(s2_d.type == 0){
                //less
                        if(s2_d.offset1 >s1_h.count){
                                errx(10,"no such address in f1");
                        }
                        if(s2_d.offset2 >s1_h.count){
                                errx(10,"no such address in f1");
                        }

                        if( lseek(fd1, sizeof(s1_h)+(s2_d.offset1*sizeof(uint64_t)) , SEEK_SET) == -1){
                                err(11,"error whith lseek");
                        }
                        if(read(fd1,&el1,sizeof(el1)) == -1){
                                err(7,"error while reading");
                        }
                        if( lseek(fd1, sizeof(s1_h)+(s2_d.offset2*sizeof(uint64_t)) ,SEEK_SET) == -1){
                                 err(11,"error whith lseek");
                        }
                        if(read(fd1,&el2,sizeof(el2)) == -1){
                                err(7,"error while reading");
                        }
                         //check if that is the right way to compare
                        if(el1>el2){
                                lseek(fd1,sizeof(s1_h)+(s2_d.offset1*sizeof(uint64_t)) ,SEEK_SET);
                                if(write(fd1,&el2, sizeof(el2)) != sizeof(el2)){
                                        err(12,"error while writing");
                                }
                                lseek(fd1,sizeof(s1_h)+(s2_d.offset2*sizeof(uint64_t)) ,SEEK_SET);
                                if(write(fd1,&el1, sizeof(el1)) != sizeof(el2)){ //sizeof(el1)??
                                        err(12,"error while writing");
                                }

                        }

                }
                else{
                //bigger
                        if(s2_d.offset2 >s1_h.count){
                                errx(10,"no such address in f1");
                        }
                        if(s2_d.offset1 >s1_h.count){
                                errx(10,"no such address in f1");
                        }
                        if ( lseek(fd1,sizeof(s1_h)+(s2_d.offset1*sizeof(uint64_t)), SEEK_SET)== -1){
                                err(11,"error whith lseek");
                        }
                        if ( read(fd1,&el1,sizeof(el1)) == -1){
                                err(7,"error while reading");
                        }
                        if ( lseek(fd1,sizeof(s1_h)+(s2_d.offset2*sizeof(uint64_t)) , SEEK_SET)== -1){
                                err(11,"error whith lseek");
                        }
                        if ( read(fd1,&el2,sizeof(el2)) == -1){
                                err(7,"error while reading");
                        }
                         //check if that is the right way to compare
                         if(el1 < el2){
                                lseek(fd1,sizeof(s1_h)+(s2_d.offset1*sizeof(uint64_t))  ,SEEK_SET);
                                if (write(fd1,&el2,sizeof(el2)) != sizeof(el2)){
                                        err(12,"error while writing");
                                }
                                lseek(fd1,sizeof(s1_h)+(s2_d.offset2*sizeof(uint64_t)) ,SEEK_SET);
                                if (write(fd1,&el1,sizeof(el1)) != sizeof(el2)){
                                         err(12,"error while writing");
                                }
                        }
                }
        }

        close(fd1);
        close(fd2);
        return 0;
}
/*Зад. 78 2022-SE-01 Напишете програма на C, която приема два позиционни аргумента – имена на
двоични файлове. Примерно извикване: ./main data.bin comparator.bin
Файлът data.bin се състои от две секции – 8 байтов хедър и данни. Структурата на хедъра е:
• uint32_t, magic – магическа стойност 0x21796F4A, която дефинира, че файлът следва тази
спецификация;
• uint32_t, count – описва броя на елементите в секцията с данни.
Секцията за данни се състои от елементи – uint64_t числа.
Файлът comparator.bin се състои от две секции – 16 байтов хедър и данни. Структурата на хедъра
е:
• uint32_t, magic1 – магическа стойност 0xAFBC7A37;
• uint16_t, magic2 – магическа стойност 0x1C27;
• комбинацията от горните две magic числа дефинира, че файлът следва тази спецификация;
• uint16_t, reserved – не се използва;
• uint64_t, count – описва броя на елементите в секциата с данни.
Секцията за данни се състои от елементи – наредени 6-торки:
• uint16_t, type – възможни стойности: 0 или 1;
• 3 бр. uint16_t, reserved – възможни стойности за всяко: 0;
• uint32_t, offset1;
• uint32_t, offset2.
Двете числа offset дефинират отместване (спрямо N0) в брой елементи за data.bin; type дефинира
операция за сравнение:
• 0: “по-малко”;
• 1: “по-голямо”.
Елементите в comparator.bin дефинират правила от вида:
• “елементът на offset1” трябва да е “по-малък” от “елементът на offset2”;
• “елементът на offset1” трябва да е “по-голям” от “елементът на offset2”.
Програмата трябва да интепретира по ред дефинираните правила в comparator.bin и ако правилото
не е изпълнено, да разменя in-place елементите на съответните отмествания. Елементи, които са
равни, няма нужда да се разменят. */
