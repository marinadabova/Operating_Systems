//Koпирайте файл /etc/passwd в текущата ви работна директория и променете разделителят на копирания файл от ":", на "?"

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>


int main(int argc,char*argv[]){
	if (argc != 2){
		err(1,"One argument needed");
	}
	int fd=open("/etc/passwd",O_RDONLY);

	if (fd == -1){
		err(2,"Error while opening");
	}
	int fd_path=open(argv[1],O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR|S_IROTH|S_IRGRP);
	if (fd_path == -1){
		err(2,"Error while opening");
	}
	int r_stat;
	char c;
	while( (r_stat=read(fd,&c,sizeof(c))) != -1 && r_stat != 0){
		if(c == ':'){
			c='?';
		}
		if(write(fd_path,&c,1) != 1){
			err(3,"Error while writing");
		}

	}
	close(fd);
	close(fd_path);

	exit(0);
}
