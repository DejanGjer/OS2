#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

void ispistrojke(int mode) {
    char res[] = "rwx";
     if (! (mode & S_IXOTH))
        res[2] = '-';
     if (! (mode & S_IWOTH))
        res[1] = '-';
     if (! (mode & S_IROTH))
        res[0] = '-';
    
     printf("%s",res);
}

void dozvole(int mode) {
    ispistrojke(mode >> 6);
    ispistrojke(mode >> 3);
    ispistrojke(mode);
}

void printFileType(struct dirent *data){
	int type = data->d_type;
	if(type == DT_DIR){
		printf("%s", "d ");
	} else {
		printf("  ");
	}
}

void printDateTime(struct tm *lc){
	int year = lc->tm_year + 1900;
	int month = lc->tm_mon + 1;
	int day = lc->tm_mday;

	int hour = lc->tm_hour;
	int min = lc->tm_min;

	printf("%d-%02d-%02d %02d:%02d", year, month, day, hour, min);	
}

void printFile(struct dirent *data, struct stat *info){
	int size = (int) info->st_size;
	char *filename = data->d_name;
	printFileType(data);
	dozvole(info->st_mode);
	int nlink = info->st_nlink;
	printf("\t%d\t%d\t", nlink, size, filename);
	struct tm *lc = localtime(&(info->st_ctime));
	printDateTime(lc);
	printf(" %s\n", filename);
}

void listDir(char *dirpath){
	DIR *dir;
	struct dirent *fileData;
	dir = opendir(dirpath);
	int e_code = errno;
	if(dir != NULL){
		while((fileData = readdir(dir)) != NULL){
			struct stat info;
			char filepath[200];
			strcpy(filepath, dirpath);
			strcat(filepath, "/");
			strcat(filepath, fileData->d_name); 
			if(stat(filepath, &info) == 0){
				printFile(fileData, &info);
			} else {
				e_code = errno;
				printf("Erorr while retrieving file stat %s: %s\n", filepath, strerror(e_code)); 
			}
		}
	} else {
		printf("Erorr while opening directory %s: %s\n", dirpath, strerror(e_code)); 
	}
}


int main(int argc, char **argv){
	for(int i = 1; i < argc; i++){
		listDir(argv[i]);
	}
}
