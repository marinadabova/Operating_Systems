#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
int main( int argc, char* argv[]){
	
	if (argc != 2){
		errx(1,"Wrong number of parameters");
	}

	int fd = open(argv[1],O_RDWR);
	if(fd < 0){
		err(2,"Error while opening file");
	}
	
	uint64_t countingSort[256] = {0};

	uint8_t buf;
	ssize_t read_bytes= read(fd, &buf, sizeof(buf));

	while ( read_bytes > 0){
		countingSort[buf]++;

	}
	if(read_bytes < 0){
		err(3,"Error while reading from file");
	}

	if(lseek(fd, 0, SEEK_SET) < 0){

		err(4,"Error while lseeking");
	}

	for (int i = 0; i < 256; i++){
		uint8_t c=i;
		for(int j = 0; j < countingSort[i]; j++){
			if (write(fd, &c, sizeof(c)) < 0){
				err(5,"Error while writing to file");
			}
		}
	}

	close(fd);


	return 0;
}



/*Зад. 58 2016-SE-01 Напишете програма на C, която приема параметър - име на (двоичен) файл с байтове.
Програмата трябва да сортира файла. */
