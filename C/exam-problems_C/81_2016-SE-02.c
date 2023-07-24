#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>

int main(void) {
    while (1){
        char command[4096];
        ssize_t num_bytes;

        // Output prompt
        write(1, "Command: ", 9);

        if ( (num_bytes=read(0, command, sizeof(command))) <= 0){
            err(1, "error reading");
        }
        if (command[num_bytes - 1] == '\n'){
            command[num_bytes - 1] = '\0';
        }
        if (strcmp(command, "exit") == 0) {
            break;
        }

        pid_t pid = fork();
        if (pid == -1) {
            err(2, "Error with fork");
        }
        else if (pid == 0) {
            if (execlp(command, command, NULL) == -1) {
                err(3, "Error with execlp");
            }
        } 
        else{

            int status;
            waitpid(pid, &status, 0);
            if (!WIFEXITED(status)){
                err(4, "Error with wait");
            }
        }
    }

    exit(0);
}

/*Зад. 81 2016-SE-02 Напишете програма на C, която реализира simple command prompt. Тя изпълнява в
цикъл следната поредица действия:
1. Извежда промпт на стандартния изход.
2. Прочита име на команда.
3. Изпълнява без параметри прочетената команда.
Командите се търсят в директорията /bin. За край на програмата се смята въвеждането на exit. */
