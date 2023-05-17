//Да се напише програма на C, която изпълнява команда date.

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(void){
	if (execl("/bin/date","date",(char*)NULL) == -1){
		err(1,"err");
	}

	exit(0);
}
