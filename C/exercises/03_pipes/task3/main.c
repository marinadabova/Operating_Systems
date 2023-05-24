//Напишете програма на C, която демонстрира употребата на dup/dup2 между parent и child процеси. Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, където той да може да се чете от stdin. Child процесът да изпълняваwc -c.
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int pf[2];
//pf[1] write parent
//pf[0] read child
	if (pipe(pf) == -1) { //suzdavame si pipe-a
		err(1, "Could not create pipe");
	}

	const pid_t child_pid = fork();  //forkvame
	if (child_pid == -1) {
		err(1, "Could not fork.");
	}

	if (child_pid == 0) { //child
		close(pf[1]); //zatvarqme kraq za pisane
		close(0); //zatvarqme standratniq vhod
		dup(pf[0]); //dublikirame standartniq vhod s kraq na trubata za chetene koeto realno shte go izpulniv na wc koqto po default chete ot standartniq vhod
		close(pf[0]);// trqbva da zatvorim pf[0]
//		sleep(5);
		if (execlp("wc", "wc", "-c", NULL) == -1) {
			err(1, "Could not exec.");
		}
	}
//parent
	close(pf[0]);//zatvarqme kraq za chetene
	write(pf[1], argv[1], strlen(argv[1])); //pishem v kraq za pisane  purviq argument
	close(pf[1]);//zatvarqme i kraq za pisane
	wait(NULL);//chakame deteto da prikluchi izpulnenieto si

	exit(0);
}

//iskame da izpulnim wc -c bez argument t.e to chaka ot stdin-a 
//
