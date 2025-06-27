#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	pid_t child = fork();
	if(child < 0){
		printf("Something did not work out.\n");
	}else if (child == 0){
		printf("I am a child whose PID is %d.\n", getpid());
	} else{
		int w =	wait(NULL);
		// waitpid(-1, NULL, 0); same as wait 
		// waitpid(-getpid(), NULL, 0); - of group id 
		// waitpid(child, NULL, 0); specific id to fetch
		// waitpid(0, NULL, 0); same process group id
		printf("I have reaped my child whose PID was %d.\n", w );
		exit(0);
	}
}
