#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	pid_t child = fork();
	if(child < 0){
		printf("Something did not work out.\n");
	}else if (child == 0){
		printf("I am child.\n");
	} else{
		printf("I am parent.\n");
		wait(NULL);
	}
}
