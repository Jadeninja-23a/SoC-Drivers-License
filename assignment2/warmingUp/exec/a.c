#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
//	char *myargs[3];
//	myargs[0] = strdup("ls");
//	myargs[1] = strdup("-l");
//	myargs[2] = NULL;
//	execvp(myargs[0], myargs);
	execlp("ls", "ls", "-l", NULL);
}
