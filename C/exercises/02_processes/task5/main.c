//Да се напише програма на C, която е аналогична на горния пример, но принуждава бащата да изчака сина си да завърши.

#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(void){
	int n=10;
	int status;

	pid_t a=fork();

	if (a == -1){
		err(1,"err");
	}
	if (a > 0){
		wait(&status);
		for(int i=0;i<n;i++){
			printf("parent\n");
		}

	}
	else{
		for(int i=0;i<n;i++){
			printf("child\n");
		}

	}
	exit(0);
}
