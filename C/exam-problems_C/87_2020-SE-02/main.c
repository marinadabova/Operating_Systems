#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1,"wrong number of arguments");
	}
	int pf[2];
	if(pipe(pf) == -1){
		err(2,"error with pipe");
	}

	pid_t p=fork();
	if(p < 0) {
		err(3,"error with fork");
	}
	else if( p == 0){
		close(pf[0]);
		if(dup2(pf[1],1) == -1){
			err(4,"error with dup2");
		}
		if(execlp("cat","cat",argv[1],(char*) NULL) < 0){
			err(5,"error with execlp");
		}
	}
	close(pf[1]);
	int fd=open(argv[2], O_CREAT| O_TRUNC| O_RDWR, S_IRUSR, S_IWUSR);
	if(fd < 0){
		err(6,"error while opening file");
	}
	ssize_t bytes_read;
	uint8_t buf;

	while( (bytes_read=read(pf[0],&buf, sizeof(buf))) >0){
		if(buf == 0x7D){
			if(read(pf[0], &buf, sizeof(buf)) < 0 ){
				err(7,"error while reading");
			}
			uint8_t temp=buf ^ 0x20;
			if (temp != 0x00 && temp != 0x55 && temp != 0xFF && temp != 0x70){
				errx(9,"wrong file format");
			}
			if(write(fd, &temp, sizeof(temp)) != sizeof(buf)){
				err(8,"error while writing");
			}
		}
		else if(buf == 0x55){
			continue;
		}
		else{ //ima li nujda?
			if(write(fd, &buf, sizeof(buf)) != sizeof(buf)){
				err(8,"error while writing");
			}
			  
		}
	}
	if(bytes_read < 0){
		err(7,"error while reading");
	}
	close(pf[0]);
	close(fd);


	return 0;
}


/*Зад. 87 2020-SE-02 При изграждане на система за пренасяне на сериен асинхронен сигнал върху ради-
опреносна мрежа се оказало, че големи поредици от битове само нули или само единици смущават
сигнала, поради нестабилно ниво. Инженерите решили проблема, като:
• в моментите, в които няма сигнал от серийният порт, вкарвали изкуствено байт 0x55 в потока;
• реалните байтове 0x00, 0xFF, 0x55 и 0x7D се кодирали посредством XOR-ване (побитова обра-
ботка с изключващо-или) с 0x20, като полученият байт се изпращал през потока, предхождан
от 0x7D, който играе ролята на escape character.
Разполагате със запис от такъв поток. Напишете програма на C, която приема два параметъра -
имена на файлове. Примерно извикване:
$ ./main input.lfld output.bin
Програмата трябва да обработва записа и да генерира output.bin, който да съдържа оригинал-
ните данни. Четенето на входните данни трябва да става посредством изпълнение на външна shell
команда. */
