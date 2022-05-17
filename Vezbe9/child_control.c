#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

bool working = false;
bool child_finished = false;

void handler(int signum){
	if(signum == SIGUSR1){
		working = true;
	} else if(signum == SIGUSR2){
		working = false;	
	} else if(signum == SIGINT){
		child_finished = true;
	}
}

void child_execution(){	
	struct sigaction siga;
	int n = 0;
	siga.sa_handler = *handler;
	siga.sa_flags = 0;
	while(true){
		if(child_finished){
			printf("Child finished execution!\n");
			exit(EXIT_SUCCESS);
		} else {
			printf("Second: %d \nChild is %sworking!\n", n,  (working) ? "" : "not ");
		}
		sleep(1);
		n++;
	}
}

int main(){
	pid_t pid;
	pid = fork();
	if(pid == -1){
		printf("Error while creating child process %s: ", strerror(errno));
		exit(EXIT_FAILURE);
	}	

	if(pid == 0){
		struct sigaction siga;
		siga.sa_handler = handler;
		siga.sa_flags = 0;
		int res = sigaction(SIGUSR1, &siga, NULL);
		if(res != 0){
			perror("Error with sigaction\n");
		}
		res = sigaction(SIGUSR2, &siga, NULL);
		if(res != 0){
			perror("Error with sigaction\n");
		}
		res = sigaction(SIGINT, &siga, NULL);
		if(res != 0){
			perror("Error with sigaction\n");
		}
		child_execution();
	} else {	
		sleep(1);
		int res = kill(pid, SIGUSR1);
		if(res != 0){
			perror("Error while sending signal\n");
		}
		sleep(3);
		res = kill(pid, SIGUSR2);
		if(res != 0){
			perror("Error while sending signal\n");
		}
		sleep(2);
		res = kill(pid, SIGINT);
		if(res != 0){
			perror("Error with sigaction\n");
		}
	}

}

