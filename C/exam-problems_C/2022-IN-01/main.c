
int main(int agrc,char* argv[]){

	int pd1[2],pd2[2];
	if (pipe(pd1)<0){
		err(3,"err");
	}


	pid_t process_id=fork();
	if(process_id <0 ){
		err(4,"err");
	}
	
	int n = atoi(argv[1]);
	int d = atoi(argv[2]);

	if(process_id ==0){


	close(pd2[1]);//?
	close(pd1[0]);
	}
	else {
	 close(pd1[1]);//?
     close(pd2[0]);

	}
	//close(pd1[0]);
	for(int i=0;i<n;i++){
		if(process_id >0){
			if (write(1, "DING ",5)<0){
				err(5,"err writing");
			}
			if(write(pd[1],"0",1)<0){
				err(6,"err writing to pipe");
			}
		}
		else{

		char buf;
		if(read(pd1[0],&buf,1)<0){
		err(7,"err while reading from pipe");
		}
		if(write(1,"DONG\n",5) <0) // na stdout
			err(5,"err while writing form pipe");
		}
		if(write(pd2[1],"0",1)< 0){

		
//lipsva kod
	}

	if( process_id 
	//argv[1] - '0' 
	//strtol
	return 0;
}
