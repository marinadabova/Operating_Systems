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

int main(int argc, char* argv[]){
    char command[5];
    if(argc == 1){
        strcpy(command,"echo");
    }
    else if (argc==2){
        if(strlen(argv[1])>4){
            errx(1,"Invalid command");
        }
        strcpy(command,argv[1]);
    }
    else{
        errx(2,"Incorrect number of arguments");
    }

    while(true){
        char string[5];
        for(int i=0;i<5;i++){
            string[i]='\0';
        }
        int bytes_read;
        int i=0;
        char c;
        while( (bytes_read=read(0,&c,sizeof(c))) > 0){
            if( c==0x20 || c==0x0A){
                break;
            }
            if( i>=4){
                errx(3,"invalid string length");
            }
            string[i]=c;
            i++;
        }
                
        if(bytes_read <0){
            err(4,"error while reading");
        }
        if(bytes_read ==0){
            exit(0);
        }
        i=0;
        char string2[4];
        int rs;
        for(int i=0;i<4;i++){
            string2[i]='\0';
        }

        while( (rs=read(0,&c,sizeof(c)))> 0){
            if( c==0x20 || c==0x0A){
                break;
            }
            if( i>=4){
                errx(3,"invalid string length");
            }
            string2[i]=c;
            i++;
        }
                
        if(rs==0){
            if(execlp(command,command,string,(char*) NULL) == -1){
                err(5,"error while ececlp");
            }
        }
    
        if(rs == -1){
            err(4,"error while reading");
        }

        int pid=fork();
        if(pid ==0){
            if( execlp(command,command,string,string2,(char*)NULL)==-1){
                err(5,"error while ececlp");
            }
        }
        else if(pid>0){
            wait(NULL);
        }
        else{
            err(6,"error with fork");
        }
    }
    //exit(0);
}

/*Зад. 83 2017-IN-02 Напишете програма на C, която приема незадължителен параметър – име на ко-
манда. Ако не е зададена команда като параметър, да се ползва командата echo. Максималната
допустима дължина на командата е 4 знака.
Програмата чете низове (с максимална дължина 4 знака) от стандартния си вход, разделени с
интервали (0x20) или знак за нов ред (0x0A). Ако някой низ е с дължина по-голяма от 4 знака, то
програмата да терминира със съобщение за грешка.
Подадените на стандартния вход низове програмата трябва да третира като множество от пара-
метри за дефинираната команда. Програмата ви трябва да изпълни командата колкото пъти е
необходимо с максимум два низа като параметри, като изчаква изпълнението да приключи, преди
да започне ново изпълнение.
Примерни вход, извиквания и изходи:
$ cat f1
a1
$ cat f2
a2
$ cat f3
a3
$ echo -e "f1\nf2 f3" | ./main cat
a1
a2
a3
$ echo -e "f1\nf2 f3" | ./main
f1 f2
f3 */
