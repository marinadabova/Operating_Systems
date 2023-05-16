//Реализирайте команда head без опции (т.е. винаги да извежда на стандартния изход само първите 10 реда от съдържанието на файл подаден като първи параматър)

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
	if (argc != 2){
		err(1,"One argument needed!");
	}
	int fd = open(argv[1],O_RDONLY);

	if (fd == -1){
		err(2,"File cannot open!");
	}
	char c;
	int i=0;
	while (read(fd,&c,sizeof(c) == sizeof(c))){
		if (c == '\n'){
			i=i+1;
		}
		if (write(1,&c,1) == -1 ){
			err(3,"Error while writing!");
		}
		if (i == 10){
			break;
		}

	}
	close(fd);
	exit(0);
}
