#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

void printFile(struct dirent *entry, struct stat *info){
	printf("%s\t%d\t%dKB\n", entry->d_name, info->st_nlink, info->st_size / 1000);
}

void listDir(char *dirpath, int size){
	DIR *dir;
	struct dirent *entry;
	dir = opendir(dirpath);
	int e_code = errno;
	if(dir != NULL){
		while((entry = readdir(dir)) != NULL){	
			struct stat info;
			char path[PATH_MAX];
			strcpy(path, dirpath);
			strcat(path, "/");
			strcat(path, entry->d_name);	
			if(stat(path, &info) == 0){
				if(info.st_size >= 1000 * size){
					printFile(entry, &info);
				}
			} else {
				printf("Error with stat on file %s: %s\n", path, strerror(errno));
			}
		}
	} else {
		printf("Error while opening the directory: %s", strerror(e_code));
	}
	printf("Zavrsio fajlove\n");
	if(closedir(dir) != 0){
		printf("Error when closing file: %s", strerror(errno));
	}
}

int main(int argc, char **argv){
	if(argc == 1){
		printf("You must provide at least one argument");
		exit(EXIT_FAILURE);
	}
	
	long size = 5;
	
	if(argc == 3){
		char *tmp;
		size = strtol(argv[2], &tmp, 10);
		if(size == LONG_MAX || size == LONG_MIN){
			printf("Error when converting string to number: %s", strerror(errno));
			exit(EXIT_FAILURE);
		}	
	}
	printf("%d\n", (int)size); 
	listDir(argv[1], (int)size);
}
