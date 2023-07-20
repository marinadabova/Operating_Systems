#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

//not tested but should work
const int DATA = 16384;
const int USER = 2048;

typedef struct data{
    uint32_t id;
        uint16_t useless1;
        uint16_t useless2;
        uint32_t start;
        uint32_t end;
}__attribute__((packed));

int main(int argc, char* argv[]){
    if(argc !=2 ){
        errx(1,"we need one parameter only");
    }
    int fd=open(argv[1],O_RDONLY);
    if(fd == -1){
        err(2,"error while opening");
    }
    struct stat st;
    if(fstat(fd,&st)== -1){
        err(3,"error with stat");
    }
    struct data d;
    if(st.st_size% sizeof(d)!= 0){
        errx(4,"wrong size format");
    }
    uint32_t values[DATA];
    uint32_t users[USER];
    uint32_t durations[USER];

    int exist = 0;
    int logC=0;
    int uniqueC=0;
    double avg=0;
    double disp=0;
    uint32_t dur;

    int rs;

    while( (rs=read(fd,&d,sizeof(d)))== sizeof(d)){
        dur=d.end- d.start;
        exist=0;
        for(int i=0;i<uniqueC;i++){
            if(users[i]== d.id){
                if(durations[i]<dur){
                    durations[i]=dur;
                }
                exist=1;
            }
        }
         if (exist == 0){
            users[uniqueC] = d.id;
            durations[uniqueC] = dur;
            uniqueC++;
        }

        logC++;
        avg += dur;
        values[logC] = dur;
    }
    if(rs== -1){
        err(5,"error while reading");
    }

    avg=avg/logC;

    for(int i=0;i<logC;i++){
        disp+=((values[i]-avg)*(values[i]-avg));
    }
    disp=disp/logC;
    for (int i = 0; i < uniqueC; i++){
        if (durations[i] > disp){
            printf("id: %d, Longest session: %d\n", users[i], durations[i]);
        }
    }
    return 0;
}
/* Зад. 70 2019-SE-01 В приложната статистика често се използват следните описателни статистики за
дадена извадка:
35
•  средна стойност x¯ = 1 N P N i=1 xi • дисперсия D = 1 N P N i=1 (xi − x¯)
където {x1, x2, . . . , xN} са стойностите на извадката, а N е техния брой.
Напишете програма на C, която приема задължителен параметър – име на двоичен файл. Фай-
лът съдържа информация за потребители, които са влизали в системата, и се състои от наредени
петорки от следните елементи и техните типове:
• уникален потребителски идентификатор (UID) uint32_t
• запазено1 uint16_t – не се използва в задачата
• запазено2 uint16_t – не се използва в задачата
• време1 uint32_t – момент на започване на сесията (Unix time)
• време2 uint32_t – момент на завършване на сесията (Unix time)
За потребители, които са имали сесии, квадратът на продължителността на които е по-голям от
дисперсията D на продължителността на всички сесии във файла, програмата трябва да изведе на
STDOUT потребителският им идентификатор и продължителността на най-дългата им сесия.
Можете да разчитате на това, че във файла ще има не повече от 16384 записа и че в тях ще се
срещат не повече от 2048 различни потребителски идентификатора.*/
