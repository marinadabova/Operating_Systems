//Да се напише програма на C, която изпълнява команда ls с точно един аргумент.
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc,char*argv[]){
	if (argc != 2){
		err(1,"One argument needed");
	}
	if (execlp("ls","ls",argv[1],NULL) == -1){
		err(2,"Error during execution");
	}

	exit(0);
}

