#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

const int NUM_ARGUMENTS = 2;

typedef struct Argument {
	char *name;
	char *label;
	int activated;
} argument;

void initializeArguments(argument a[]) {
	argument recursive = {
		.name = "recursive",
		.label = "-R",
		.activated = 0
	};
	a[0] = recursive;

	argument listHidden = {
		.name = "list_hidden",
		.label = "-a",
		.activated = 0
	};
	a[1] = listHidden;
}

int parseArguments(int argc, char **argv, argument *args) {
	int lastArgIndex = 1;
	for(int i = 1; i < argc; i++){
		if(argv[i][0] != '-'){
			return lastArgIndex;
		}

		lastArgIndex++;

		for(int j = 0; j < NUM_ARGUMENTS; j++){
			if(strcmp(argv[i], args[j].label) == 0){
				args[j].activated = 1;
			}
		}
	}

	return lastArgIndex = 1;
}

int printFile(char *path, struct dirent *entry, argument *args);

int listDirectory(char *dirname, argument *args) {	
	int links = 0;
	DIR *dir;
	struct dirent *entry;
	dir = opendir(dirname);
	int e_code = errno;
	if(dir != NULL){
		printf("-----------------------------------------------------------------------\n");
		printf("Fajlovi u direktorijumu %s: \n", dirname);
		while((entry = readdir(dir)) != NULL){
			links += printFile(dirname, entry, args);
		}
	} else {
		printf("Desila se greska na otvaranju direktorijuma %s: %s\n", dirname, strerror(e_code)); 
	}

	int closed = closedir(dir);
	e_code = errno;
	if(closed != 0){
		printf("Desila se greska pri zatvaranju direktorijuma %s: %s\n", dirname, strerror(e_code)); 
	}
	printf("-----------------------------------------------------------------------------\n");
	return links;
}

int printFile(char *path, struct dirent *entry, argument *args) {
	int links = 0;
	char dirpath[200];
	strcpy(dirpath, path);
	if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)){
		if(args[1].activated) {			
			printf("%s\n", entry->d_name);
		}
	} else {
		if(args[0].activated && entry->d_type == DT_DIR) {
			links = listDirectory(strcat(strcat(dirpath, "/"), entry->d_name), args);
		} else {			
			printf("%s\n", entry->d_name);
		}
	}

	if(entry->d_type == DT_LNK){
		links++;
	}

	return links;
}

int main(int argc, char **argv){
	argument args[NUM_ARGUMENTS];
	initializeArguments(args);

	int lastArgIndex = parseArguments(argc, argv, args);
	for(int i = lastArgIndex; i < argc; i++){
		printf("Number of links: %d\n", listDirectory(argv[i], args));
		printf("=====================================================================\n");
	}

}
