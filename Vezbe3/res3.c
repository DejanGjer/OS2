# include <stdio.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <time.h>

// ispis sadrzaja datih direktorijuma; ispisuje se nekoliko podatak vezano za njih

// ovo je pomocna proc za tacno jednu trojku, uvek tretira kao da je najdonja
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

void list(char *name) {
    DIR *d = opendir(name);
    if (d == NULL) {
        int err = errno;
        printf("error while reading %s: %d %s\n", name,err,strerror(err));
        return;
    }
    
    // za kasnije sastavljanje putanja
    char path[PATH_MAX];
    struct stat sb;
    
    struct dirent *ent;
    
    // set errno for later checks
    errno = 0;
    while ((ent = readdir(d)) != NULL) {
    
        if (ent->d_type == DT_DIR)
            printf("d ");
        else
            printf("  ");
            
        // treba nam jos podataka - koristimo stat
        strcpy(path,name);
        strcat(path,"/");
        strcat(path,ent->d_name);
        
        int res = stat(path, &sb);
        if (res == -1) {
            printf("error %d on stat for %s: %s\n", errno,path,strerror(errno));
        } else {    
        
            dozvole(sb.st_mode);
        
            // velicina nam se daje u broju blokova po 512; 
            // mozemo to ispisati, ili cisto broj blokova, sto se vidi u ls sa "-s"
            printf("\t%ld\t %ld \t\t", sb.st_nlink, 512 * sb.st_blocks);
            
            // vreme poslednje izmene
            struct tm *lc = localtime(&(sb.st_ctime));
            
            printf("%d-%02d-%02d ", (1900 + lc->tm_year), lc->tm_mon+1, lc->tm_mday);
            printf("%02d:%02d ", lc->tm_hour, lc->tm_min);
        
            printf("%s", ent->d_name);
            printf("\n");
        }
    }
    if (errno != 0) {
        printf("error while reading dir: %d %s\n", errno,strerror(errno));       
    }
    int ret = closedir(d);
    if (ret == -1) {
        printf("error closing dir stream: %d %s\n", errno,strerror(errno));           
    }

}

int main(int argn, char **args) {
    if (argn == 1) {
        printf("No arguments given; exiting;\n");
        exit(0);
    }
    for (int i=1; i<argn; i++){
        printf(args[i]);
        printf("\n");
        list(args[i]);
    }
}
