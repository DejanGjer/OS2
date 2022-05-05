#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int factorial(int n){
	if(n == 0){
		return 1;
	}	

	int pipefd[2];
	pid_t pid;
	int wstatus;

	if (pipe(pipefd) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
        }

	pid = fork();
	if(pid == -1){
		printf("Error while creating child process %s: ", strerror(errno));
		exit(EXIT_FAILURE);
	}	

	if(pid == 0){
		close(pipefd[0]);
		int res = factorial(n - 1);
		char msg[12];
		sprintf(msg, "%d", res);
		write(pipefd[1], msg, strlen(msg)); 
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	} else {
		close(pipefd[1]);
		char buf;
		char msg[12];
		int res = 0;
		pid_t w_pid = waitpid(pid, &wstatus, 0);
		if(WIFEXITED(wstatus)){
			int i = 0;
			while(read(pipefd[0], &buf, 1) > 0){
				msg[i] = buf;
				i++;
			}	
			res = n * atoi(msg);
			//printf("%d\n", res);
		} else {
			printf("Child process didn't terminated normally! %d\n", wstatus);
		}
		close(pipefd[0]);
		return res;
	}
	return 0;
}


int main(int argc, char **argv){
	if(argc != 2){
		printf("You should provide argument for number n\n");
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
	int res = factorial(n);
	printf("%d\n", res);
}

