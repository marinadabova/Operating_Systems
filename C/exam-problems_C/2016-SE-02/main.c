#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <unistd.h>
struct pair{
	uint32_t x;
	uint32_t y;
}__attribute__((packed));


int main(int argc, char* argv[]){
	if (argc != 4) {
		errx(1,"Wrong number of arguments");
	}
	struct pair data;
	struct stat st;
	if(stat(argv[1], &st) < 0){
		err(2,"Error with stat file");
	}
	if (st.st_size %(2 *sizeof(uint32_t)) != 0) {
		 errx(3, "File1 is not in our format");
	}
	
	if(stat(argv[2], &st) < 0){
	 	err(2,"Error with stat file");
	}
	if(st.st_size % sizeof(uint32_t) != 0){ // proverqvam si size-a 
		errx(3, "File2 is not in our format");
   	}

	int fd1= open(argv[1], O_RDONLY);
	if(fd1 < 0){
		err(4, "Error while opening first file");
	}
	int fd2= open(argv[2], O_RDONLY);
	if(fd2 < 0){
		 err(4, "Error while opening second file");
	}
	
	int fd3= open(argv[3], O_CREAT, O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd3 < 0){
	 	err(4, "Error while opening  file");
	}
	ssize_t bytes_read;

	while( ( bytes_read = read(fd1, &data, sizeof(data))) > 0){
		if( lseek(fd2, (data.x)*sizeof(uint32_t) , SEEK_SET)< 0){
			err(6, "Error with lseek");
		}
		uint32_t buf;
		for(uint32_t i=0; i<data.y; i++){
			bytes_read= read(fd2, &buf, sizeof(buf));
			if(bytes_read < 0){
				err(7,"Error while reading");
			}
			if(write(fd3, &buf, sizeof(buf)) < 0){
				err(8,"Error while writing");
			}
			//if(write(fd3, &buf, sizeof(buf)) != sizeof(buf)){
			//err(8,"Error while writing");
			//}
		}
	}
	if(bytes_read < 0){
		err(7,"Error while reading");
	}

	close(fd1);
	close(fd2);
	close(fd3);
	 

	return 0;
}


/*Зад. 59 2016-SE-02 Двоичните файлове f1 и f2 съдържат 32 битови числа без знак (uint32_t). Файлът f1
е съдържа n двойки числа, нека i-тата двойка е < xi, yi >. Напишете програма на C, която извлича
интервалите с начало xi и дължина yi от файла f2 и ги записва залепени в изходен файл f3.
Пример:
• f1 съдържа 4 числа (2 двойки): 30000, 20, 19000, 10
• програмата записва в f3 две поредици 32-битови числа, взети от f2 както следва:
• най-напред се записват числата, които са на позиции 30000, 30001, 30002, ... 30019.
• след тях се записват числата от позиции 19000, 19001, ... 19009.
Забележка: С пълен брой точки ще се оценяват решения, които работят със скорост, пропорцио-
нална на размера на изходния файл f3. */
