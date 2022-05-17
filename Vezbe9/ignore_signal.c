#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

int main(){
	struct sigaction siga;
	int n = 0;
	siga.sa_handler = SIG_IGN;
	while(true){
		int res = sigaction(SIGINT, &siga, NULL);
		if(res != 0){
			perror("Error with sigaction\n");
		}
		sleep(1);
		printf("Sleepig for %d seconds\n", n);
		n++;
	}
}
