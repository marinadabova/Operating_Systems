//Да се напише програма на C, която създава процес дете и демонстрира принцина на конкурентност при процесите.
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
	pid_t a=fork();

	if (a == -1){
		err(1,"err");
	}
	if (a > 0){
		for(int i=0;i<n;i++){
			write(1,"Parent",1);
		}
	}
	else{
		for(int i=0;i<n;i++){
			write(1,"Child",1);
		}
	}
	write(1,"\n",1);
	exit(0);
}
