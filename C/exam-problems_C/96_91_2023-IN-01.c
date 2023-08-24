#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
        if(argc != 3){
                errx(1,"We need two arguments");
        }
        if(strlen(argv[1]) > 1){
                errx(2,"[0-9[ number needed");
        }
        if(strlen(argv[2]) > 1){
                errx(2,"[0-9[ number needed");
        }

        int N=atoi(argv[1]);
        int D=atoi(argv[2]);

        if (N <0 || N>9){
                errx(3, "We need number in [0,9]");
        }
        if( D<0 || D>9){
                errx(3, "We need number in [0,9]");
        }
        int cw[2]; // we will use this pipe to signal the child
        int pw[2]; //we will use this pipe to signal parent that the child completed its writing to stdout
        if(pipe(cw) <0){
                err(4,"Error with pipe");
        }
        if(pipe(pw) <0){
                err(4,"Error with pipe");
        }
        pid_t pid=fork();
        if(pid <0 ){
                err(5,"Error while fork");
        }
        if(pid ==0){
                //child
                close(cw[1]);
                close(pw[0]);

                for(int i=0;i<N;i++){
                        char buf;
                        if(read(cw[0],&buf,1) <0){ //the child is reading the signal from parent from pipe cw
                                err(8,"Error while reading from pipe");
                        }
                        if(write(1,"DONG\n",5) != 5){ //child writing to stdout
                                err(6,"Error while writing");
                        }
                        if(write(pw[1],"D",1) != 1){ //child is writing signal (random D) to the pw pipe to signal the parent that its action is completed
                                err(7,"Error while writing signal to parent");
                        }

                        if(i==N){
                                exit(0); //prekratqva child processa??
                                //idk if its necessary
                        }
                }
        }
        else{
                //parent

                close(cw[0]);
                close(pw[1]);

                for(int i=0;i<N;i++){
                        if(write(1,"DING ",5) !=5 ){ //parent is writing to stdout
                                err(6,"Error while writing");
                        }
                        if(write(cw[1],"D", 1) !=1){ //parent signal with random D the child when writing to cw pipe
                                err(7,"Error while writing signal to child");
                        }
                        char buf;
                        if(read(pw[0],&buf,1)< 0){ //parent is reading signal from the child if the child is completed
                                err(8,"Error while reading from pipe");
                        }
                        sleep(D); //sleeping
                }
        }
        wait(NULL); //waiting for the child ? //idk if necessary

        close(pw[0]);
        close(cw[1]);


        exit(0);
}
