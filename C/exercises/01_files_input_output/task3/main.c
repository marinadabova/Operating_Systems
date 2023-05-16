/*Реализирайте команда swap, разменяща съдържанието на два файла,
подадени като входни параметри.
Приемаме, че двата файла имат еднакъв брой символи.
*/
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char* argv[]){
	if (argc != 3){
		err(1,"Two arguments needed");
	}
	int fd1 = open(argv[1],O_RDWR);
	if ( fd1 == -1 ){
		err(2,"Error while opening the first file");
	}

	int fd2 = open(argv[2],O_RDWR);
	if ( fd1 == -1 ){
		err(2,"Error while opening the second file");
    }

    int fd3=open("temp", O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
    if (fd3 == -1){
    	close(fd1);
    	close(fd2);
    	err(3,"Error while opening temp file");
    }
    char c[4096];
	ssize_t read_size;

	while ((read_size = read(fd1,&c,sizeof(c))) > 0 ){
		if (write(fd3,&c,read_size) != read_size){
			err(4,"Error while writing");
		}
	}	
	if (read_size != 0){
		err(5,"Error while reading");
	}

	lseek(fd1,0,SEEK_SET);
		
	while ((read_size = read(fd2,&c,sizeof(c))) > 0 ){
         if (write(fd1,&c,read_size) != read_size){
             err(4,"Error while writing");
         }
    }
    if (read_size != 0){
        err(5,"Error while reading");
	}

	lseek(fd2,0,SEEK_SET);
	lseek(fd3,0,SEEK_SET);
	
	while ((read_size = read(fd3,&c,sizeof(c))) > 0 ){
		if (write(fd2,&c,read_size) != read_size){
              err(4,"Error while writing");
        }
    }
    if (read_size != 0){
    	err(5,"Error while reading");
    }
    close(fd1);
    close(fd2);
	close(fd2);

	exit(0);
}
