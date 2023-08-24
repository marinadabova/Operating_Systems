
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
                errx(1, "wrong number of arguments");
        }
        if(strlen(argv[1]) >1){
                errx(2, "[0-9]number needed");
        }
        if(strlen(argv[2]) >1){
                errx(2, "[0-9]number needed");
        }

        int N=argv[1][0] - '0';
        int D=argv[2][0] - '0';
        //int N = atoi(argv[1]);
        //int D= atoi(argv[2]);

        if(N < 0 || N > 9){
                errx(2, "We need number in [0,9]");
    }

    if(D < 0 || D > 9){
        errx(3, "We need numebr in [0,9]");
    }

        //if( N <= 0){
        //      errx(4,"Nothing to be printed");
        //}

        char parent[5]="DING ";
        char child[5]="DONG\n";
        int p1[2]; //unblock_child
        int p2[2]; //unblock_parent

        int a=42;

        if(pipe(p1) == -1){
                err(4,"error with pipe");
        }
        if(pipe(p2) == -1){
                err(4,"error with pipe");
        }

        pid_t pid=fork();
        if(pid == -1){
                err(5,"error fork");
        }
        if(pid == 0){
                close(p1[0]);
                close(p2[1]);

                for(int i=0;i<N;i++){
                        // Child process reads from p2
                        if(read(p2[0],&a,sizeof(a)) == -1){
                                err(6,"error while reading");
                        }
                        // Child process writes to stdout
                        if(write(1, &child, 5) != 5){
                                err(7, "error while writing child");
                        }
                         // Child process writes to p1
                        //signal parent:
                        if(write(p1[1], &a, sizeof(a)) != sizeof(a)){
                                err(7,"error while writing c");
                        }
                }
        }
        else{
                close(p1[1]);
                close(p2[0]);
                
                for(int i=0;i<N;i++){
                    // Parent process writes to stdout
                        if(write(1,&parent,5) != 5){
                                err(7,"error while writing parent");
                        }
                        // Parent process writes to p2
                        if(write(p2[1],&a,sizeof(a)) !=sizeof(a)){
                                err(7,"error while writing p");
                        }
                        // Parent process reads from p1
                        if(read(p1[0], &a, sizeof(a)) == -1){
                                err(6, "error while reading");
                        }
                        sleep(D);
                }
        }
        close(p2[1]);
        close(p1[0]);
        return 0;
}

/* Зад. 90 2022-IN-01 Ваши колеги - асистенти по ОС имат нужда от демострационна програма на C, която
да служи като пример за конкурентност и синхронизация на процеси. Напишете такава програма,
приемаща два задължителни позиционни параметъра – едноцифрени числа. Примерно извикване:
./main N D
Общ алгоритъм на програмата:
• началният (родителски) процес създава процес-наследник
• N на брой пъти се изпълнява:
– родителският процес извежда на stdout низа “DING ”
– процесът-наследник извежда на stdout низа “DONG”
– родителският процес изчаква D секунди
Гарантирайте, че:
• процесът-наследник винаги извежда “DONG” след като родителския процес е извел “DING ”
• родителският процес винаги започва изчакването след като процеса-наследник е извел “DONG”
Забележка: За изчакването погледнете sleep(3).*/
