// cat /etc/passwd | grep 5 | sort

#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>


int main(void){

	int fd1[2];
	int fd2[2];

	pipe(fd1);
	pipe(fd2);

	pid_t cat_pid;
	pid_t grep_pid;


	if( (cat_pid = fork()) == -1){
		err(1,"Err cat");	
	}
	if(cat_pid == 0) {
		close(fd1[0]);
		//1: fd1[1]

		if ( dup2(fd1[1],1) == -1 ){
			err(2,"dup2 cat err");
		}
		if(execlp("cat","cat", "/etc/passwd",NULL) == -1){
			err(3,"exec cat err");
		}
		

	}
	close(fd1[1]);

	if((grep_pid =fork()) == -1){
		err(4,"err grep");
	}
	if(grep_pid == 0){
		close(fd2[0]);

		if( dup2(fd1[0],0) == -1){
			err(5,"dup2 grep  err");
		}

		if( dup2(fd2[1],1) == -1){
 			err(6,"dup2 grep  err");
		}
	    if(execlp("grep","grep", "5",NULL) == -1){
			err(7,"exec grep err");
		}


	}
	close(fd2[1]);

	if(dup2(fd2[0],0) == -1){
		err(9,"err dup2");
	}
	execlp("sort","sort",NULL);

	err(10,"err exec sort");
	exit(0);
}
