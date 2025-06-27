#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

volatile sig_atomic_t bg_process_finished = 0;


/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void SIGCHLD_handler(int sig){
	int savederrno = errno;
    int status;
    pid_t pid;
    char msg[128];

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        int len = snprintf(msg, sizeof(msg), "\nShell: Background process %d finished\n", pid);
        write(STDOUT_FILENO, msg, len);
		bg_process_finished = 1;

        if (WIFEXITED(status)) {
            len = snprintf(msg, sizeof(msg), "EXITSTATUS: %d\n", WEXITSTATUS(status));
            write(STDOUT_FILENO, msg, len);
        }
		len = snprintf(msg, sizeof(msg), "\n$ ");
		write(STDOUT_FILENO, msg, len);
    }
	errno = savederrno;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	char cwd[256];
	bool bgProcess = 0;
	int noOfTokens = 0;
	char* lastToken = NULL;
	signal(SIGCHLD, SIGCHLD_handler);
	while(1) {			
		/* BEGIN: TAKING INPUT */
		bgProcess = 0;
		bzero(line, sizeof(line));
		if (getcwd(cwd, sizeof(cwd)) != NULL){
			printf("%s$ ", cwd);
			fflush(stdout);
		}else{
			printf("(Cant fetch current directory or path is to long)$ ");
			fflush(stdout);
		}
		scanf("%[^\n]", line);
		getchar();


		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
		if(tokens[0] == NULL){
			for(i=0;tokens[i]!=NULL;i++){
				free(tokens[i]);
			}
			free(tokens);
			continue;
		}	
  		noOfTokens = 1;
		for(int i = 0;tokens[i] != NULL; i++){
			lastToken = tokens[i];
			noOfTokens = i+1;
		} 
		if(strcmp(lastToken, "&") == 0){
			bgProcess = 1;
		}
       //do whatever you want with the commands, here we just print them
		if(strcmp(tokens[0], "cd") == 0){
			if(tokens[2] != NULL){
				printf("cd: too many arguments.\n");
			}else{
				int error = chdir(tokens[1]);
				if(error){
				printf("cd: path not found.\n");
				}
			}
		
		}else {
		
      			pid_t f = fork();
		       	if(f < 0){
				printf("System Error occured exiting Shell Program. . . . ..\n");
				exit(0);
			}else if (f==0){
				if(bgProcess){
					free(tokens[noOfTokens - 1]);
					tokens[noOfTokens-1] = NULL;

				}
				execvp(tokens[0], tokens);

				for(i=0;tokens[i]!=NULL;i++){
					free(tokens[i]);
				}
				free(tokens);
				exit(1);
			}else{
				if(!bgProcess){
					int status;
					waitpid(f,&status,0);
					if(WEXITSTATUS(status)){
						printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
					}
				}
				
				
			}
		}
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
