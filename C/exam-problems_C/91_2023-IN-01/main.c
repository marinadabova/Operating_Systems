#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

const char L[3][4]={"tic ", "tac ","toe\n" };
int main(int argc, char* argv[]){
	if(argc !=3){
		errx(1,"wrong number of arguments");
	}

	int pds[8][2];
	int NC=atoi(argv[1]);
	int WC=atoi(argv[2]);

	if(NC < 1 ||NC > 7){
		errx(2,"wrong input");
	}
	if(WC < 1 ||WC > 35){
		errx(2,"wrong input");
	}

	for(int i=0;i<NC;i++){
		if(pipe(pds[i]) == -1){
			err(3,"error with pipe");
		}
	}
	int id=NC;
	pid_t pid=0;
	for(int i=0;i<NC;i++){
		pid=fork();
		if(pid == -1){
			err(4,"error with fork");
		}
		if(pid ==0){
		// not neccessary,  i will remain value according to a specific child
			id=i;
			break;
		}
	}
	if(pid >0){
		for(int i=1;i<NC;i++){
			close(pds[i][0]);
			close(pds[i][1]);
		}
		close(pds[0][0]);
		close(pds[NC][1]);
	}
	else{
		for(int i=0;i<NC+1;i++){
			if(i == id){
			 	close(pds[i][1]);
			 	close(pds[i+1][0]);
				i++;
			}
			else{
				close(pds[i][0]);
				close(pds[i][1]);
			}
		}
	}

	int count=0;

	if(id == NC){
		if(write(1,L[count%3],4) == -1){
			err(5,"error while writing to stdout");
		}
		count++;
		if(write(pds[0][1], &count, sizeof(count)) == -1){
			err(6,"error while writing to pipe");
		}
	}

	while(read(pds[id][0], &count, sizeof(count))) {
		if(count == WC){
			break;
		}
		if(write(1, L[count%3], 4) == -1){
			err(6,"err while writing to pipe");
		}
		count++;
		if(id == NC){
			if(write(pds[0][1], &count, sizeof(count)) == -1){
				err(6,"err while writing to pipe parent");
			}
		}
		else{
			if(write(pds[id+1][1], &count, sizeof(count)) == -1) {
				 err(6,"err while writing to pipe child");
			}
		}
	}

	if(id == NC){
		 close(pds[0][1]);
		 close(pds[id][0]);
	}
	else{
		close(pds[id+1][1]);
		close(pds[id][0]);
	}



	return 0;
}

/* Зад. 91 2023-IN-01
Ваши колеги – асистенти по ОС – имат нужда от демострационна програма на C, която да служи
като пример за конкурентност и синхронизация на процеси.
Дефиниран е нареден списък с три думи L = (”tic ” , ”tac ” , ”toe\n”), като всяка дума е с дължина
четири знака. Напишете програма, приемаща две числа като аргументи (./main NC WC), като NC
е ограничено в интервала [1, 7], а WC – в интервала [1, 35]. Програмата трябва задължително да
работи по следния общ алгоритъм:
• началният (родителски) процес създава NC на брой процеси-наследници;
• групата процеси извеждат на stdout общо WC на брой думи от горния списък при следните
правила:
– ако броят на думите за извеждане WC е по-голям от общия брой думи в L, след изчер-
пване на думите в L програмата започва списъка отначало колкото пъти е нужно;
43
– всеки процес извежда само една дума;
– първата дума се извежда от родителския процес;
– ако броят на думите за извежданеWC е по-голям от общия брой процеси, след изчерпване
на процесите програмата започва да ги ползва от начало. Например при родителски
процес P и два процеса-наследници C1 и C2 редът на използване на процесите ще бъде
P, C1, C2, P, C1, C2, P, C1, ...
– изведените думи гарантирано трябва да са по реда, дефиниран в L;
– всеки процес гарантирано започва да извежда на stdout поредната дума, след като пред-
ходния процес е приключил с извеждането на предишната.
Забележка: За конвертиране на аргументите погледнете strtol(3). */
