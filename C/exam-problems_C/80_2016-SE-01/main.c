#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main( int argc, char* argv[]){
	if(argc != 2){
		errx(1,"wrong number of argumenst");
	}
	//int fd=open(argv[1], O_RDONLY);
	//if(fd == -1){
	//	err(2,"error while opening");
	//}

	int pf[2];
	if(pipe(pf) == -1){
		err(2,"error with pipe");
	}
	pid_t p=fork();
	if(p < 0){
		err(3,"error with fork");
	}
	else if(p == 0){
		close(pf[0]);
		if(dup2(pf[1],1) == -1){
			err(5,"error with dup2");
		}
		if(execlp("cat","cat", argv[1], (char*)NULL) == -1){
			err(4,"error with execlp");
		}
	}
	//parent is reading from pipe and writing to sort
	close(pf[1]);
	if(dup2(pf[0],0) == -1){
		err(5,"error with dup2");
	}

	//close(pf[0]); ??

	if(execlp("sort", "sort", (char*)NULL) == -1){
		err(4,"error with execlp");
	}

	wait(NULL);
	exit(0);
}

/*Зад. 80 2016-SE-01 Напишете програма на C, която по подадено име на (текстови) файл като параметър,
извежда съдържанието на файла сортирано, чрез употреба на външните програми cat и sort през
pipe(). */
