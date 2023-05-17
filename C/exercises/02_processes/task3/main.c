//Да се напише програма на C, която изпълнява команда sleep (за 60 секунди).

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(void){
	if (execlp("sleep","sleep","60",NULL) == -1){
		err(1,"error while executing");
	}
	exit(0);
}
