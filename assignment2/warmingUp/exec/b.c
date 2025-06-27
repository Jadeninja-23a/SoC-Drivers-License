#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	pid_t child = fork();
	if(child < 0){
		printf("Something did not work out.\n");
	}else if (child == 0){
		if(argc != 3){
			printf("Incorrect number of arguments.\n");
			exit(0);
		}
		execlp(argv[1], argv[1], argv[2], NULL);
	} else{
		int w =	wait(NULL);
	}
}
