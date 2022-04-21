#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv){
	int buf_size = PATH_MAX * sizeof(char);
	char *path = (char*) malloc(buf_size);
	if(argc == 1){
		path = getcwd(path, PATH_MAX);
		int e_code = errno;
		if(path == NULL){
			printf("Error while getting current path: %s", strerror(e_code));
			exit(0);
		}
	} else {
		path = realpath(argv[1], path);	
		int e_code = errno;
		if(path == NULL){
			printf("Error while building absolute path: %s", strerror(e_code));
			exit(0);
		}
	}
	printf("Absolute path: %s\n", path);
	free(path);
}
