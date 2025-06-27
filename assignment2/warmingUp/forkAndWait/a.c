#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(char argc, char *argv[]){
	int N = atoi(argv[1]);
	int expectedProcesses = (1<<N) - 1;
	int childProcesses = 0;
	printf("Predicting %d no. of child processes.\n", expectedProcesses);

	for(int i =0;i < N;i++){
	int pid = fork();
	if(pid < 0){
		printf("Sorry fork didnt work.\n");
	}else if(pid ==0){
		printf("My PID is %d.\n",(int) getpid());
		childProcesses = 0;
	} else {
		childProcesses ++;	
	}
	}
	for(int i =0; i<childProcesses;i++){
		int pid = wait(NULL);
		printf("Child process with PID: %d was reaped.\n", pid);
	}

}
