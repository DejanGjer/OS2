#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv){
	int rez = mkdir("./os2", 0777);
	int error_code = errno;
	if(rez == 0){
		printf("Folder uspesno napravljen");
	} else {
		printf(strerror(error_code));
	}
	return 0;
}
