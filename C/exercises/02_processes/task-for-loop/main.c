//Прегледайте прикачената програма на C във fork-loop/main.c и отговорете на следните въпроси:

/*В какъв ред ще се отпечатат символните низове на всеки от процесите? Пробвайте да изпълните програмата няколко пъти.
Какво ще се изпише на стандартния изход, ако само разкоментираме exit(0); в детинските процеси?
В какъв ред ще се отпечатат низовете, ако само разкоментираме wait(NULL);? Редът един и същ ли ще е винаги? Защо?
Какво ще се отпечата на стандартния изход, ако разкоментираме и exit(0);, и wait(NULL);?
Колко реда ще се отпечатат, ако N = 3 при обстоятелствата от предния въпрос?
Колко реда ще се отпечатат, ако N = 3, но exit(0); е закоментиран? При тези обстоятелства, как нараства броят на отпечатаните редове при растене на N? (Съвет: НЕ пробвайте да изпълнявате програмата с "голямо" N)
Ще има ли memory leak, ако разменим реда на free(indentation); и разкоментирания exit(0);?
Ще има ли memory leak, ако махнем всичките 2 реда free(indentation); и оставим само един free(indentation); на последния ред във for цикъла? Ако да, при какви ситуации?
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

char* indent(int tabs);

char* indent(int tabs) {
	char * indentation = (char *) malloc(tabs + 1);

	for (int i = 0; i < tabs; ++i) {
		indentation[i] = ' ';
	}

	indentation[tabs] = '\0';
	return indentation;
}

const int N = 3;

int main(void) {
	for (int i = 0; i < N; ++i) {
		// int i = 2; in C++ will be problem
		int pid = fork();
		char * indentation = indent(i);

		if (pid > 0) {
//wait(NULL);
			printf("%s%d: I'm parent PID: %d PPID: %d\n",
				indentation, i, getpid(), getppid());

			// 1 - uncomment to order the tree

//			wait(NULL);
			
			/*
			printf("%s%d: I'm parent PID: %d PPID: %d done waiting\n",
				indentation, i, getpid(), getppid());
			*/
			free(indentation);
		} else if (pid == 0) {

			printf("%s%d: I'm child PID %d PPID: %d\n",
				indentation, i, getpid(), getppid());

			//exit(0);
			free(indentation);


			// 2 - uncomment to create a vine
			exit(0);

		}
		//free(indentation);

	}
}
