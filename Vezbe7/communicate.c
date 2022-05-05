#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
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
		char* msg = "Message";
		write(pipefd[1], msg, strlen(msg)); 
		close(pipefd[1]);
	} else {
		close(pipefd[1]);
		char buf;
		pid_t w_pid = waitpid(pid, &wstatus, 0);
		if(WIFEXITED(wstatus)){
			while(read(pipefd[0], &buf, 1) > 0){
				printf("%c", buf);
			}	
		} else {
			printf("Child process didn't terminated normally!\n");
		}
		close(pipefd[0]);
	}

}

