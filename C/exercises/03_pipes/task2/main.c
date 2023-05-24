//Напишете програма на C, която демонстрира комуникация през pipe между parent и child процеси. Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, който да го отпечатва на стандартния изход.

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>


int main( int argc,char* argv[]){
	if (argc != 2) {
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int pf[2];

//pf[0] -> child -> read
//pf[1] -> parent -> write

	if (pipe(pf) == -1) {
		errx(2, "Could not create pipe");
	}
	const pid_t child_pid = fork();
	if (child_pid == -1) {
		err(3, "Could not fork.");
	}
	if (child_pid == 0) { // namirame se v child-a
		close(pf[1]); //zatvarqme kraq koito ne polzvame(na parenta write) za da nqma deadlock
		char buf; //buffer

		while (read(pf[0], &buf, 1) > 0) { //chetem ot bufera  ot 0(t.e kraq na trubata)
			write(1, &buf, 1);
		}
		close(pf[0]);
		exit(0);
	}
//tozi kod se izpulnqva v parent-a

	close(pf[0]); //zatvarq kraq za read
	write(pf[1], argv[1], strlen(argv[1])); 

	close(pf[1]);
	wait(NULL); //za da ne stane child-a zombie process

	exit(0);
}
/*
Ako ne ztavorim kraq za pisane  close(pf[1]); read-a na 35-ti red  ne znae koga da priklucki i produljava do bezkrai da chaka kraq za pisane na parent-a da pishe neshto
*/
