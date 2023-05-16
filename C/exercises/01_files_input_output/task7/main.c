/*Напишете програма, която приема точно 2 аргумента. Първият може да бъде само --min, --max или --print (вижте man 3 strcmp). 
Вторият аргумент е двоичен файл, в който има записани цели неотрицателни двубайтови числа (uint16_t - вижте man stdint.h). 
Ако първият аргумент е:

--min - програмата отпечатва кое е най-малкото число в двоичния файл.
--max - програмата отпечатва кое е най-голямото число в двоичния файл.
--print - програмата отпечатва на нов ред всяко число.
Използвайте двоичния файл binary/dump или генерирайте сами такъв (правилно подравнен).

*/

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char*argv[]){
	if(argc != 3){
		err(1,"Two arguments needed");
	}
	int option1=strcmp(argv[1],"--min");
 	int option2=strcmp(argv[1],"--max");
 	int option3=strcmp(argv[1],"--print");
	
	if(option1 != 0 && option2 !=0 && option3 !=0){
		err(2,"False param");
	}
	uint16_t c, min=UINT16_MAX,max=0;
	int r_stat;
	
	int fd=open(argv[2],O_RDONLY);
	if(fd == -1){
		err(3,"Error while opening");
	}
	while( (r_stat=read(fd,&c,sizeof(c))) != 0){
		if(r_stat== -1){
			err(4,"Error while reading");
		}
		if(c > max){
			max=c;
		}
		if(min > c){
			min=c;
		}
		if(option3 == 0){
			printf("%d\n",c);
		
		}
	}

		if(option1 == 0){
			printf("Min: %d\n",min);
		}
		else if(option2 == 0){
			printf("Max: %d\n",max);
		}

		close(fd);

	exit(0);
}
