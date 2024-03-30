
#include <unistd.h>
#include <err.h>
#include <stdlib.h>

const char L[3][4] = { "tic ", "tac ", "toe\n" };

int main(int argc,char* argv[]){
        if(argc != 3){
                errx(1,"Wrong argument count");
        }

        int pds[8][2]; //[8] number of pipes [2]ppc za pipe-a kkoeto si e
        // 8 na broi zashtoto iskame 0 za parent 7 za children nai mn -sprqmo NC
        int NC=atoi(argv[1]);
        int WC=atoi(argv[2]);

        if(NC <1 || NC> 7){
                errx(2,"Number of children should be between 1 and 7");
        }
        if(WC<1 || WC >35){
                errx(2,"Number of words should be between 1 and 35");
        }

        for(int i=0;i<NC+1 ;i++){ //A loop is used to create NC + 1 pipe pairs
        //we want NC+1 because we want 0 to be for the parent and 7 for children
                if(pipe(pds[i])== -1){
                        err(3,"Error creating pipe");
                }
        }
        int id=NC; //3
        pid_t pid=0;

        for(int i=0;i<NC;i++){
                //A loop is used to fork NC child processes.
                //Each child process has its own ID (0 to NC-1), and the parent process has ID NC.
                pid=fork();
                if(pid <0){
                        err(4,"Error while fork");
                }
                if(pid == 0){
                        // not neccessary,  i will remain value according to a specific child
                        id=i;// id-to e 0,1,2 do NC-1 za child processite
                        break;//why- ne iskame child-a da suzdava svoi deca
                        //ako sme c childa- ne pravi oshte childove na childa(shot toi shte se vurne gore pri fork-a) ami spri do tuk
                }
        }
        if(pid >0){
                for(int i=1;i<NC;i++){
                        //In the parent process, all unnecessary pipe descriptors are closed.
                        //It keeps the first and last pipe descriptors open for communication with the child processes.
                        close(pds[i][0]);
            close(pds[i][1]);
        }
        close(pds[0][0]);
        close(pds[NC][1]);
    }
    else{
        for(int i = 0; i < NC+1; i++){
                if(i == id){ //0 == 0 demek sme v purvoto dete
                        close(pds[i][1]); // zatvarqm pds[0][1] demek shte chetem ot 0 truba
                close(pds[i+1][0]); //zatvarqm pds[1][0] demek shte pishem ot 1 truba
                i++;
            }
            else {
                close(pds[i][0]);
                close(pds[i][1]);
            }

        }
    }
    int count = 0; //The variable count is used to keep track of the number of words outputted. It is initially set to 0.
    if(id == NC){
        if(write(1, L[count % 3], 4) == -1) {
        //In the parent process (ID = NC), it writes the first word from  L to stdout
                err(4, "Error while writing");
        }
        count++;      //Then it increments count and writes it to the pipe connected to the first child process.
        if(write(pds[0][1], &count, sizeof(count)) == -1) {
              err(2, "writing to pipe %d", count);
        }
    }
        while(read(pds[id][0], &count, sizeof(count))) { //Each child process enters a loop where it reads the value of count from its pipe descriptor.
        if(count == WC) { //If count reaches the value of WC, the loop is terminated.
                break;
        }
        if(write(1, L[count % 3], 4) == -1) { //Within the loop, each child process writes the word from the L list based on the current value of count to the standard output.
                err(3, "writing to pipe ");
        }
        count++; // It then increments count
        if(id == NC) {
                if(write(pds[0][1], &count, sizeof(count)) == -1) { //and writes it to the next child's pipe or back to the parent process if it is the last child.
                        err(3, "writing to pipe parent" );
            }
        }
        else {
                if(write(pds[id+1][1], &count, sizeof(count)) == -1) { //next child is that
                        err(3, "writing to pipe child");
            }
        }
    }

    if(id == NC) {
        close(pds[0][1]);
        close(pds[id][0]);
    }
    else {
        close(pds[id+1][1]);
        close(pds[id][0]);
    }
        exit(0);
}
