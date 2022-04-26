#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv){
	printf("Sadrzaj trenutnog direktorijuma\n");
	int result = system("ls -gG");
	if(result == -1){
		printf("Child process could not be created: %s\n", strerror(errno));
	} else if(result == 127){
		printf("Error in executing command in child process\n");
	} else { 
		printf("Kraj ispisa\n");
	}	

}
