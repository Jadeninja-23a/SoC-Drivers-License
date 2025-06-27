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
#define MAX_COMMANDS 64

pid_t childPids[MAX_TOKEN_SIZE];
pid_t fgchildPids[MAX_COMMANDS];
int fgchildPidsIndex = 0;
int bgPidsIndex = 0;
volatile sig_atomic_t interrupted = 0;

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

void sigint_handler(int sig){
	//do nothing
	interrupted = 1;//Indirectly resets the for loop and prevents it from happening
}

void SIGCHLD_handler(int sig){
	int savederrno = errno;
    int status;
    pid_t pid;
    char msg[128];
	for(int i = 0; i < bgPidsIndex; i++){
    	pid = waitpid(childPids[bgPidsIndex], &status, WNOHANG);
		if(pid <= 0){
			continue;
		}
        int len = snprintf(msg, sizeof(msg), "\nShell: Background process %d finished\n", pid);
        write(STDOUT_FILENO, msg, len);
		
			if(pid == childPids[i]){
				for(int j = i; j<bgPidsIndex-1;j++){
					childPids[j] = childPids[j+1];
				}
				bgPidsIndex--;
				childPids[bgPidsIndex] = -1;
				break;
			}
		
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
	char ***allTokens;
	int tokenNo = 0;
	int currentT = 0;
	int mode = 0; //Where 0 stands for serial processing and 1 stands for parallel processing          
	int i;
	char cwd[256];
	int counter = 0;
	bool bgProcess = 0;
	int noOfTokens = 0;
	char* lastToken = NULL;
	signal(SIGINT,sigint_handler);
	signal(SIGCHLD, SIGCHLD_handler);
	while(1) {			
		/* BEGIN: TAKING INPUT */
		bgProcess = 0;
		mode=0;
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

		for(int i =0; i < MAX_COMMANDS;i++){
			fgchildPids[i] = -1;
		}
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
			if(strcmp(tokens[i], "&&&") == 0){
				mode = 1;
				break;
			}
		} 
		if(strcmp(lastToken, "&") == 0){
			bgProcess = 1;
		}
		char *token;
		if(mode){
			token = "&&&";
		} else{
			token = "&&";
		}
		allTokens = malloc(64 * sizeof(char **));
		for(int i =0; i<64;i++){
			allTokens[i] = malloc(MAX_NUM_TOKENS * sizeof(char *));
		}
		tokenNo = 0;
		counter = 0;
		for(int i = 0; tokens[i] != NULL; i++){
			if(strcmp(tokens[i], token) == 0){
				
				allTokens[tokenNo][counter] = NULL;
				tokenNo++;
				counter = 0;
			}else{
				allTokens[tokenNo][counter++] = tokens[i];
			}
		}
		allTokens[tokenNo][counter] = NULL;
		allTokens[tokenNo+1] = NULL;
		if(!mode){
		for(int i = 0; i<=tokenNo;i++){
			if(interrupted){
				interrupted = 0;
				break;
			}
       //do whatever you want with the commands, here we just print them

		if(strcmp(allTokens[i][0], "cd") == 0){
			if(allTokens[i][2] != NULL){
				printf("cd: too many arguments.\n");
			}else{
				int error = chdir(allTokens[i][1]);
				if(error){
				printf("cd: path not found.\n");
				}
			}
		
		}else if(strcmp(allTokens[i][0], "exit") == 0){
			if(allTokens[i][1] != NULL){
				printf("exit: too many arguments.\n");
				continue;
			}
			for(int j=0;allTokens[i][j]!=NULL;j++){
				free(allTokens[i][j]);
			}
			free(allTokens[i]);
			printf("Exiting program.......\n");
			for(int j =0;i<bgPidsIndex;j++){
				kill(childPids[j], SIGKILL);
				waitpid(childPids[j],NULL,0);
			}
			exit(0);

		}else {
		
      			pid_t f = fork();
		       	if(f < 0){
				printf("System Error occured exiting Shell Program. . . . ..\n");
				exit(0);
			}else if (f==0){
				if(bgProcess){
					free(allTokens[i][noOfTokens - 1]);
					allTokens[i][noOfTokens-1] = NULL;
					setpgid(0, 0);
				}
				execvp(allTokens[i][0], allTokens[i]);

				for(int j=0;allTokens[i][j]!=NULL;j++){
					free(allTokens[i][j]);
				}
				free(allTokens[i]);
				exit(1);
			}else{
				if(!bgProcess){
					int status;
					waitpid(f,&status,0);
					if(WEXITSTATUS(status)){
						printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
					}
				}else{
					childPids[bgPidsIndex++] = f;
				}
				
				
			}
		}
		
	}
    } else {
	for(int i = 0; i<=tokenNo;i++){
     
		if(strcmp(allTokens[i][0], "cd") == 0){
			if(allTokens[i][2] != NULL){
				printf("cd: too many arguments.\n");
			}else{
				int error = chdir(allTokens[i][1]);
				if(error){
				printf("cd: path not found.\n");
				}
			}
		
		}else if(strcmp(allTokens[i][0], "exit") == 0){
			if(allTokens[i][1] != NULL){
				printf("exit: too many arguments.\n");
				continue;
			}
			for(int j=0;allTokens[i][j]!=NULL;j++){
				free(allTokens[i][j]);
			}
			free(allTokens[i]);
			printf("Exiting program.......\n");
			for(int j =0;i<bgPidsIndex;j++){
				kill(childPids[j], SIGKILL);
				waitpid(childPids[j],NULL,0);
			}
			exit(0);

		}else {
			
      		pid_t f = fork();
		    if(f < 0){
				printf("System Error occured exiting Shell Program. . . . ..\n");
				exit(0);
			}else if (f==0){
				if(bgProcess){
					free(allTokens[i][noOfTokens - 1]);
					allTokens[i][noOfTokens-1] = NULL;
					setpgid(0, 0);
				}
				execvp(allTokens[i][0], allTokens[i]);

				for(int j=0;allTokens[i][j]!=NULL;j++){
					free(allTokens[i][j]);
				}
				free(allTokens[i]);
				exit(1);
			}else{
				if(!bgProcess){
					fgchildPids[i] = f;
				}else{
					childPids[bgPidsIndex++] = f;
				}
				
				
			}

		}
		
	}
	fgchildPidsIndex = tokenNo;
	int flag = 0;
	while(fgchildPidsIndex > -1){
		for(int i = 0; i<=tokenNo;i++){
			flag = 0;
			if(fgchildPids[i] == -1){
				continue;
			}
			flag = 1;
			int status;
			pid_t pid;
			pid = waitpid(fgchildPids[i], &status, 0);
			if(pid > 0){
				fgchildPids[i] = -1;
				fgchildPidsIndex--;
			}
		}	
		if(flag){
			break;
		}
	}
	}	
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		for (int i = 0; i < MAX_COMMANDS; i++) {
    		free(allTokens[i]);
		}
		free(allTokens);

	}
	return 0;
}
