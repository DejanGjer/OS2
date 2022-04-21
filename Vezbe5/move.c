#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>

void createDestination(char *dirpath){
	if(access(dirpath, F_OK) == 0){
		return;
	}

	//mode_t mode = S_IRUSR || S_IXUSR || S_IWGRP;
	mode_t mode = 0777;
	if(mkdir(dirpath, mode) != 0){
		printf("Error while creating directory\n");
	}
}

int checkFile(char *dirpath, char *filename){
	DIR *dir;
	struct dirent *entry;
	dir = opendir(dirpath);
	int e_code = errno;
	if(dir != NULL){
		while((entry = readdir(dir)) != NULL){
			if(strcmp(entry->d_name, filename) == 0){
				return 1;
			}	
		}
	} else {
		printf("Error while opening directory %s: %s", dirpath, strerror(e_code));
	}
	if(closedir(dir) != 0){
		printf("Error while closing directory %s,: %s", dirpath, strerror(errno));
	}
	return 0;
}

char* pathJoin(char *path1, char *path2){
	char *fullpath = (char*) malloc(PATH_MAX * sizeof(char));
	strcpy(fullpath, path1);
	strcat(fullpath, "/");
	strcat(fullpath, path2);
	return fullpath;
}

int main(int argc, char **argv){
	if(argc < 4){
		printf("Not enough parameters\n");
	}

	char *dir_origin = argv[1];
	char *dir_destination = argv[2];
	createDestination(dir_destination);	

	for(int i = 3; i < argc; i++){
		if(checkFile(dir_origin, argv[i]) == 0){
			printf("File %s, does not exist", argv[i]);
		} else {			
			char *origin = pathJoin(dir_origin, argv[i]);
			char *destination = pathJoin(dir_destination, argv[i]);
			if(rename(origin, destination) != 0){
				printf("Error while moving file %s to %s: %s", origin, destination, strerror(errno));
			} else {
				if(chmod(destination, 0500) != 0){
					printf("Error with chmod: %s", strerror(errno));
				}
			}
					
			free(origin);
			free(destination);
		}
	}
}
