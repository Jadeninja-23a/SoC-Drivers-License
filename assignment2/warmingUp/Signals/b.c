#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

static volatile sig_atomic_t got_signal = 0;

void notInterrupted(int signall){
	got_signal = 1;
}

int main(){
	
	signal(SIGINT, notInterrupted);
//	struct sigaction sa;
//	memeset(&sa, 0, sizeof(struct sigaction));
//	sa.sa_handler = &notInterrupted;
//	sigemptyset(&sa.sa_mask);
//	sa.sa_flags =0;
//	sigaction(SIGINT, &act, 0);
	while(1){
		if(got_signal){
			printf("Haha You though you could stop me with CTRL+C.\n");
			sleep(1);
			got_signal = 0;
		}
		printf("I will run forever \n");
	}
}
