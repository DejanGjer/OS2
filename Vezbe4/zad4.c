#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

const int NUM_ARGUMENTS = 4;

typedef struct argument{
	char *name;
	char *label;
	bool activated;
	bool composite; 
} Argument;

char znak;
int year;
char *curr_dir;

char* pathJoin(const char *path1, const char *path2){
	char *fullpath = (char*) malloc(PATH_MAX * sizeof(char));
	strcpy(fullpath, path1);
	strcat(fullpath, "/");
	strcat(fullpath, path2);
	return fullpath;
}

void initializeArguments(Argument *args){
	Argument sort = {
		.name = "sort",
		.label = "-sort",
		.activated = false,
		.composite = false
	};
	args[0] = sort;

	Argument start = {
		.name = "startswith",
		.label = "-S",
		.activated = false,
		.composite = true
	};
	args[1] = start;

	Argument year = {
		.name = "year",
		.label = "-p",
		.activated = false,
		.composite = true
	};
	args[2] = year;

	Argument month = {
		.name = "month",
		.label = "-",
		.activated = false,
		.composite = false
	};
	args[3] = month;
}

int findMonth(char *name){
	if(strcmp(name, "jan") == 0){
		return 0;
	} else if(strcmp(name, "feb") == 0){
		return 1;
	} else if(strcmp(name, "mart") == 0){
		return 2;
	} else if(strcmp(name, "april") == 0){
		return 3;
	} else if(strcmp(name, "maj") == 0){
		return 4;
	} else if(strcmp(name, "jun") == 0){
		return 5;
	} else if(strcmp(name, "jul") == 0){
		return 6;
	} else if(strcmp(name, "avgust") == 0){
		return 7;
	} else if(strcmp(name, "septembar") == 0){
		return 8;
	} else if(strcmp(name, "oktobar") == 0){
		return 9;
	} else if(strcmp(name, "novembar") == 0){
		return 10;
	} else if(strcmp(name, "decembar") == 0){
		return 11;
	}
	return -1;
}

int filter_a(const struct dirent *entry){
	if(entry == NULL){
		return 0;
	} else if(entry->d_name[0] == 'a'){
		return 1;
	}
	return 0;
}


int filter_N(const struct dirent *entry){
	if(entry == NULL){
		return 0;
	} else if(entry->d_name[0] == 'N'){
		return 1;
	}
	return 0;
}

int filter_znak(const struct dirent *entry){
	if(entry == NULL){
		return 0;
	} else if(entry->d_name[0] == znak){
		return 1;
	}
	return 0;
}

int filter_year(const struct dirent *entry){
	if(entry == NULL){
		return 0;
	}
	
	char *filepath = pathJoin(curr_dir, entry->d_name);
	struct stat info;
	if(stat(filepath, &info) != 0){
		printf("Stat ne radi za %s: %s\n", filepath, strerror(errno));
	}
	struct tm *lt = localtime(&(info.st_ctime));
	printf("%d\n", lt->tm_year);
	int given_year = lt->tm_year + 1900;
	if(given_year > year){
		return 1;
	}
	printf("%d, %d\n", year, given_year);
	return 0;
}

int sort_inode(const struct dirent **el1, const struct dirent **el2){
	if(el1 == NULL){
		return -1;
	}
	if(el2 == NULL){
		return 1;
	}
	return (int)((*el1)->d_ino - (*el2)->d_ino);
}	

void listDir(char *dirpath, 
		int (*filter)(const struct dirent *), 
		int (*sort_comp)(const struct dirent **, const struct dirent **)){

	struct dirent **stavke;
	int broj;

	broj = scandir(dirpath, &stavke, filter, sort_comp);
	if(broj == -1){
		perror("Greska kod scandira\n");
		return;
	}

	for(int i = 0; i < broj; i++){
		struct dirent *stavka = stavke[i];
		if(stavka->d_type == DT_DIR){
			printf("d ");
		} else {
			printf("  ");
		}	
		printf("%s - %ld\n", stavka->d_name, stavka->d_ino);
		free(stavke[i]);
	}
	free(stavke);
}

int main(int argc, char **argv){
	if(argc == 1){
		printf("Treba proslediti bar parametar sa direktorijumom koji se izlistava");
		exit(0);
	}

	Argument args[NUM_ARGUMENTS];
	initializeArguments(args);

	int (*odabrani)(const struct dirent *) = NULL;
	int (*sort)(const struct dirent **, const struct dirent **) = NULL;

	if(strncmp(argv[1], "--S", 3) == 0){
		znak = argv[1][3];
		odabrani = filter_znak;
	} else if(strncmp(argv[1], "-p", 2) == 0){
		char *tmp;
		year = (int) strtol(argv[1], &tmp, 10);
		printf("By year: %d\n", year);
		odabrani = filter_year;
	} else if(strcmp(argv[1], "--sort") == 0){
		sort = alphasort;
	} else if(strcmp(argv[1], "--sort2") == 0){
		sort = sort_inode;
	} else {
		printf("Pogresan prvi argument\n");
	}

	for(int i = 2; i < argc; i++){
		curr_dir = argv[i];
		listDir(argv[i], odabrani, sort);
	}
}
