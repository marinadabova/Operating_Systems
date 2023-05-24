/*
Да се напише програма на C, която приема аргумент - име на файл. Програмата да:

-записва във файла 'fo'
-създава child процес, който записва 'bar\n'
-parent-а, след като изчака child процеса, записва 'o\n'

*/

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char*argv[]){
	if (argc != 2){
		err(1,"Invalid number of argumnets");
	}
	const int fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (fd == -1){
		err(2,"Error while opening");
	}
	const char* str1="foo\n";
	const char* str2="bar\n";
	if(write(fd,str1,2)!=2) {// dva baita zapisvame
		err(3,"Could not write two chars from str1");
	}

	const pid_t child_pid=fork();
	if (child_pid == -1){
		err(4,"Could not fork");
	}
	if (child_pid == 0){
		if(write(fd,str2,4) != 4){
			err(5,"Could not write four chars from child");
		}
		exit(0);
	}

	int child_status;
	const pid_t wait_code = wait(&child_status);

	if (wait_code == -1) {
		err(6, "Could not wait for child.");
	}

	if (!WIFEXITED(child_status)) {
		err(7, "Child did not terminate normally.");
	}

	if (WEXITSTATUS(child_status) != 0) {
		err(8, "Child exit code not 0, file should be already closed.");
	}

	if (write(fd, str1+2, 2) != 2) {
		err(9, "Could not write the last two chars from parent");
	}

	close(fd);
	exit(0);
}
