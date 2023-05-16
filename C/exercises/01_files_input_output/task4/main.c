//Реализирайте команда cat, работеща с произволен брой подадени входни параметри.

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>


int main (int argc, char*argv[] ){
	if (argc < 2){
		err(1,"You need at least one argument");
	}
	int fd;
	char c;
	int r_stat,w_stat;
	for (int i=1;i<argc;i++){
		fd=open(argv[i],O_RDONLY);
		if (fd == -1){
			err(2,"Error while opening");
		}
		while ((r_stat=read(fd,&c,sizeof(c))) != -1 && r_stat!=0){
			if ((w_stat=write(1,&c,1)) == -1){
				err(3,"Error while writing");
			}
		}
		close(fd);
	}
	exit(0);
}
