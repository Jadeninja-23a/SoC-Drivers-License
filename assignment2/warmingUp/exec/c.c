#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
	printf("Heyy heyy not badd.\n");
	execlp("ls", "ls", "-l", NULL);
	printf("What IS WRONG WITH U  CANT EVEN RUN A EXEC COMMMAND,\n");
}
