#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

bool interrupted = false;

void handler(int signum){
	interrupted = true;
}

int main(){
	struct sigaction siga;
	int n = 0;
	siga.sa_handler = *handler;
	while(true){
		int res = sigaction(SIGINT, &siga, NULL);
		if(res != 0){
			perror("Error with sigaction\n");
		}
		if(interrupted){
			printf("Process is interrupted\n");
			exit(EXIT_SUCCESS);
		} else {
			sleep(1);
			printf("Sleepig for %d seconds\n", n);
			n++;
		}
	}
}

