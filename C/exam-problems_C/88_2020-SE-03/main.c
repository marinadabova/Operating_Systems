#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define N 8
struct data{
	char name[8];
	uint32_t offset;
	uint32_t length;
}__attribute__((packed));

int main(int argc, char* argv[]){
	if( argc != 2){
		errx(1,"wrong number of arguments");
	}
	struct data d;
	struct stat st;
	if(stat(argv[1],&st) <0){
		err(2,"error while stat");
	}
	if(st.st_size % sizeof(d) != 0 || (long unsigned int)st.st_size > 8*sizeof(d)){
		errx(3,"wrong format");
	}
	int fd=open(argv[1], O_RDONLY);
	if( fd == -1){
		err(4,"error while opening");
	}
	//?
	int count= st.st_size /sizeof(d);
	if(count > N){
		errx(5,"wrong format file");
	}

	struct data arr[8];
	int p[2];
	if(pipe(p)< 0){
		err(6,"error with pipe");
	}

	pid_t x;
	int i;

	for(i=0; i<count;i++){
		if(read(fd, &arr[i], sizeof(d)) != sizeof(d)){
			err(7,"error while reading");
		}
		x=fork();
		if( x== -1){
			err(8,"error with fork");
		}
		if (x==0){
			break;
		}
	}

	if(x==0){
		close(p[0]);
		uint16_t result_child=0;
		uint16_t buf=0;
		int curfd=open(arr[i].name, O_RDONLY);
		if(curfd == -1){
			err(4,"error while opening");
		}
		struct stat st1;

		if(stat(arr[i].name , &st1) < 0){
			err(2,"error while stat");
		}

		if(st1.st_size % sizeof(uint16_t) !=0){
			errx(3,"wrong format");
		}
		if(lseek(curfd,arr[i].offset *sizeof(uint16_t), SEEK_SET) <0){
			err(9,"error with lseek");
		}

		for(uint32_t j=0; j< arr[i].length ;j++){
			if(read(curfd, &buf, sizeof(buf)) != sizeof(buf)){
				err(7,"error while reading");
			}
			result_child= result_child ^ buf;
		}

		if(write(p[1], &result_child, sizeof(result_child)) != sizeof(result_child)){
			err(10,"error while writing to pipe");
		}
		exit(0);
	}

	close(p[1]);
	uint16_t result=0;
	uint16_t buff;

	for(int j=0;j<count;j++){
		// wait(NULL);
		if(read(p[0], &buff, sizeof(buff)) != sizeof(buff)) {
			err(7,"error while reading");
		}
		result=result ^ buff;
	}
	printf("result: %dB\n", result);

	close(fd);

	exit(0);
}


/*Зад. 88 2020-SE-03 Напишете програма на C, която приема един задължителен позиционен параметър
- име на файл. Файлът се състои от не повече от 8 наредени тройки елементи:
• име на файл – точно 8 байта, последният от които задължително е 0x00. Ако името е по-късо
от 7 знака, излишните байтове са 0x00;
• offset – uint32_t, който дава пореден номер на елемент (спрямо N0) във файла;
• length – uint32_t, който дава брой елементи.
За всяка наредена тройка програмата трябва да пусне child процес, който да XOR-не (обработи с
изключващо-или) елементите (uint16_t) от съответния файл един със друг, и да върне резултата
на parent процеса, който от своя страна трябва да XOR-не всички получените резултати и да изведе
полученото число в следния формат:
result: 573B
Забележка: С пълен брой точки се оценяват решения, в които child процесите работят паралелно. */
