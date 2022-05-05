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
		printf("Child\n");
	} else {
		pid_t w_pid = waitpid(pid, &wstatus, 0);
		if(WIFEXITED(wstatus)){
			printf("Parent\n");
		} else {
			printf("Child process didn't terminated normally!\n");
		}
	}

}

