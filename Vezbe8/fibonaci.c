#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int fibonaci(int n){
	if(n == 0){
		return 0;
	}	
	if(n == 1){
		return 1;
	}

	int pfd1[2];
	int pfd2[2];
	pid_t pid1;
	pid_t pid2;
	int wstatus;

	if(pipe(pfd1) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
        }
	
	if(pipe(pfd2) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
        }

	pid1 = fork();
	if(pid1 == -1){
		printf("Error while creating child process %s: ", strerror(errno));
		exit(EXIT_FAILURE);
	}	

	if(pid1 != 0){
		pid2 = fork();
		if(pid2 == -1){
			printf("Error while creating child process %s: ", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}	

	if(pid1 == 0){
		close(pfd1[0]);
		int res = fibonaci(n - 1);
		char msg[12];
		sprintf(msg, "%d", res);
		//printf("Child %d - poruka %s\n", n - 1, msg);
		write(pfd1[1], msg, strlen(msg)); 
		//printf("%s\n", msg);
		close(pfd1[1]);
		exit(EXIT_SUCCESS);
	} else if(pid2 == 0){
		close(pfd2[0]);
		int res = fibonaci(n - 2);
		char msg[12];
		sprintf(msg, "%d", res);
		//printf("Child %d - poruka %s\n", n - 2, msg);
		write(pfd2[1], msg, strlen(msg)); 
		//printf("%s\n", msg);
		close(pfd2[1]);
		exit(EXIT_SUCCESS);
	} else {
		close(pfd1[1]);
		close(pfd2[1]);
		char buf;
		char msg1[12];
		char msg2[12];
		int res = 0;
		pid_t w_pid1 = waitpid(pid1, &wstatus, 0);
		if(WIFEXITED(wstatus)){
			int i = 0;
			while(read(pfd1[0], &buf, 1) > 0){
				msg1[i] = buf;
				i++;
			}	
			//printf("%d fibonaci u parentu: %s\n", n - 1, msg1); 
			res += atol(msg1);
		} else {
			printf("Child process didn't terminated normally! %d\n", wstatus);
		}

		pid_t w_pid2 = waitpid(pid2, &wstatus, 0);
		if(WIFEXITED(wstatus)){
			int i = 0;
			while(read(pfd2[0], &buf, 1) > 0){
				msg2[i] = buf;
				i++;
			}	
			//printf("%d fibonaci u parentu: %s\n", n - 1, msg1); 
			res += atol(msg2);
		} else {
			printf("Child process didn't terminated normally! %d\n", wstatus);
		}
		close(pfd1[0]);
		close(pfd2[0]);
		//printf("%d fibonaci je: %d\n", n, res);
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
	int res = fibonaci(n);
	printf("%d\n", res);
}

