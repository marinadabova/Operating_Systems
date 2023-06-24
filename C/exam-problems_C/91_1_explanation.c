// Зад. 89 2023-IN-01 Ваши колеги – асистенти по ОС – имат нужда от демострационна програма на C, която да служи като пример за конкурентност и синхронизация на процеси.
// Дефиниран е нареден списък с три думи L = (”tic ” , ”tac ” , ”toe\n”), като всяка дума е с дължина четири знака. Напишете програма, приемаща две числа като аргументи (./main NC WC), 
// като NC е ограничено в интервала [1, 7], а W C – в интервала [1, 35]. Програмата трябва задължително да работи по следния общ алгоритъм:
// • началният (родителски) процес създава NC на брой процеси-наследници;
// • групата процеси извеждат на stdout общо W C на брой думи от горния списък при следните правила:
// – ако броят на думите за извеждане W C е по-голям от общия брой думи в L, след изчерпване на думите в L програмата започва списъка отначало колкото пъти е нужно;
// – всеки процес извежда само една дума;
// – първата дума се извежда от родителския процес;
// – ако броят на думите за извеждане W C е по-голям от общия брой процеси, след изчерпване на процесите програмата започва да ги ползва от начало. 
// Например при родителски процес P и два процеса-наследници C1 и C2 редът на използване на процесите ще бъде P, C1, C2, P, C1, C2, P, C1, ...
// – изведените думи гарантирано трябва да са по реда, дефиниран в L;
// – всеки процес гарантирано започва да извежда на stdout поредната дума, след като предходния процес е приключил с извеждането на предишната.
// Забележка: За конвертиране на аргументите погледнете strtol(3).

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>

const char L[3][4] = { "tic ", "tac ", "toe\n" };

int main(int argc, char* argv[]) {

        if(argc != 3) {
                errx(1, "Arg count");
        }

        int pds[8][2];
        int NC = atoi(argv[1]);
        int WC = atoi(argv[2]);

                if (NC < 1 || NC > 7){
                        errx(2, "#children 1-7");
                }

                if (WC < 1 || WC > 35){
                        errx(2, "#words 1-35");
                }


        for(int i = 0; i < NC + 1; i++) { //A loop is used to create NC + 1 pipe pairs  NC+1 shtot iskame 0 za parent i 7 deca
                if(pipe(pds[i]) == -1) {
                        err(2, "creating pipe");
                }
        }

        int id = NC;
        pid_t pid = 0;
        for(int i = 0; i < NC; i++) { //A loop is used to fork NC child processes. Each child process has its own ID (0 to NC-1), and the parent process has ID NC.
                pid = fork();

                                if (pid == -1) {
                                                err(4, "Cannot fork() for child# %d", i);
                                }

                if(pid == 0) {
                        // not neccessary,  i will remain value according to a specific child
                        id = i;
                        break;
                }
        }

        if(pid > 0) {
                for(int i = 1; i < NC; i++) { //In the parent process, all unnecessary pipe descriptors are closed. It keeps the first and last pipe descriptors open for communication with the child processes.
                        close(pds[i][0]);
                        close(pds[i][1]);
                }
                close(pds[0][0]);
                close(pds[NC][1]);
        } else {
                for(int i = 0; i < NC+1; i++) {
                        if(i == id) {
                                close(pds[i][1]);
                                close(pds[i+1][0]);
                                i++;
                        } else {
                                close(pds[i][0]);
                                close(pds[i][1]);
                        }
                }
        }

        int count = 0; //The variable count is used to keep track of the number of words outputted. It is initially set to 0.

        if(id == NC) { 
                if(write(1, L[count % 3], 4) == -1) { //In the parent process (ID = NC), it writes the first word from the predefined list L to the standard output (stdout). 
                
                        err(2, "writing to stdout");
                }
                count++;      //Then it increments count and writes it to the pipe connected to the first child process.
                if(write(pds[0][1], &count, sizeof(count)) == -1) {
                        err(2, "writing to pipe %d", count);
                }
        }

        while(read(pds[id][0], &count, sizeof(count))) { //Each child process enters a loop where it reads the value of count from its pipe descriptor. 
                if(count == WC) { //If count reaches the value of WC, the loop is terminated.
                        break;
                }
                if(write(1, L[count % 3], 4) == -1) { //Within the loop, each child process writes the word from the L list based on the current value of count to the standard output.
                        err(3, "writing to pipe ");
                }
                count++; // It then increments count 
                if(id == NC) {
                        if(write(pds[0][1], &count, sizeof(count)) == -1) { //and writes it to the next child's pipe or back to the parent process if it is the last child.
                                err(3, "writing to pipe parent" );
                        }
                } else {
                        if(write(pds[id+1][1], &count, sizeof(count)) == -1) { //next child is that
                                err(3, "writing to pipe child");
                        }
                }
        }

        if(id == NC) {
                close(pds[0][1]);
                close(pds[id][0]);
        } else {
                close(pds[id+1][1]);
                close(pds[id][0]);
        }

// OR exit(0) to close all of ours file descriptors
        return 0;
}

