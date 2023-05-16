//Реализирайте команда wc, с един аргумент подаден като входен параметър

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
	if (argc != 2){
		err(1,"One ergument needed!");
	}
	int fd = open(argv[1],O_RDONLY);
	if (fd == -1){
		err(2,"Error while opening!");
	}
	char c;
	int lines=0;
	int words=0;
	int chars=0;
	while ( read(fd,&c,sizeof(c) == sizeof(c)) ){
		if (c == '\n'){
			lines++;
			words++;
		}
		if (c == ' '){
			words++;
		}
		chars++;

	}
	printf("%d %d %d %s\n", lines, words, chars, argv[1]);
	close(fd);
	
	exit(0);
}
