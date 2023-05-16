//Реализирайте команда cp, работеща с произволен брой подадени входни параметри.

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>


int main (int argc, char*argv[]){
	if (argc < 3){
		err(1,"Arguments should be at least two");
	}
	int fd,r_stat;
	char c;
	int fd_path=open(argv[argc-1], O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	if (fd_path == -1){
		err(2,"Error while opening");
	}
	for(int i=1;i<argc-1;i++){
		if ( (fd=open(argv[i],O_RDONLY)) == -1){
			err(2,"Error while opening");
		}
		while ( (r_stat=(read(fd,&c,sizeof(c)))) != -1 && r_stat !=0){
			if(write(fd_path,&c,r_stat) != r_stat){
				err(3,"Error while writing");
			}
		}
		close(fd);
	}
	close(fd_path);

	exit(0);
}
