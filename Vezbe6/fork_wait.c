#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	pid_t pid;
	int wstatus;

	printf("Sadrzaj trenutnog direktorijuma\n");

	pid = fork();
	if(pid == -1){
		printf("Error while creating child process %s: ", strerror(errno));
		exit(EXIT_FAILURE);
	}	

	if(pid == 0){
		int result = execl("/bin/ls", "ls", "-gG",  NULL);
		if(result == -1){
			printf("Error in child process: %\ns", strerror(errno));
			exit(EXIT_FAILURE);
		}

	} else {
		pid_t w_pid = waitpid(pid, &wstatus, 0);
		if(WIFEXITED(wstatus)){
			printf("Kraj ispisa\n");
		} else {
			printf("Child process didn't terminated normally!\n");
		}
	}

}

