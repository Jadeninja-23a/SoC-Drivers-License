#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
	pid_t child = fork();
	if (child < 0){
		printf("FK OFF \n");
	} else if(child == 0){
		sleep(1000);
	} else {
		kill(child, SIGKILL);
		int status;
		waitpid(child, &status, 0);
		printf("Child terminated by signal %s\n", strsignal(WTERMSIG(status)));
	}
}
