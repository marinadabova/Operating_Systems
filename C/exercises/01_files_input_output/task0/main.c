// Копирайте съдържанието на file1 във file2

#include <fcntl.h> 
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char* argv[]){
	if (argc != 3){
		errx(1,"err");
	}
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(2,"Error while opening file1!");
	}
	int fd2 = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);	
	if (fd2 == -1){
		err(2,"Error while opening file2!");
	}
	
	char c;
	ssize_t bytes_read;

	while ( (bytes_read = read(fd1,&c,sizeof(c))) > 0){
		if (write(fd2,&c,sizeof(c)) == -1){
			err(3,"Error while writing!");
		}
	}
	if (bytes_read == -1){
		err(4,"Error while reading!");
	}

	close(fd1);
	close(fd2);
	exit(0);
	return 0;
}
