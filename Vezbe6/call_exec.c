#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv){
	printf("Sadrzaj trenutnog direktorijuma\n");
	int result = execl("/bin/ls", "-gG",  NULL);
	if(result == -1){
		printf("Error in child process: %\ns", strerror(errno));
	}
	printf("Kraj ispisa\n");
}
