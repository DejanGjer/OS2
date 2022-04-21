# include <dirent.h>
# include <stdio.h>

/*
Zadatak: ispisati prosledjene argumente sa komandne linije.

Potrebno je ispisati i argument 0, tj ime izvrsnog fajla kako
je pozvan.
 */

int main(int argc, char **argv) {
    printf("hello, os2\n");
    
    for (int i=0; i < argc; i++) {
        printf(argv[i]);
        printf("\n");
    }
}