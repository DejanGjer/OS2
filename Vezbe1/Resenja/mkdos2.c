# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>

/*
 Zadatak: napraviti folder pod imenom "os2" u trenutnom direktorijumu.
 Prijaviti gresku, ako je bilo.
 
 Tipicna greska je da vec postoji "os2" (ako pokrenemo program dva puta
 npr), ili da nemamo prava pristupa, ali moguce su i druge.
*/


int main () {
    if (mkdir("os2", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH) == -1) {
        // pokazacemo dva nacina ispisa greske
        
        // zapamtimo "errno" za kasnije, jer ga neka komanda moze resetovati
        int e = errno;
        
        // moze se koristiti samo komanda koja ispise podatke o poslednjoj gresci
        perror("");
        
        // alternativno se moze iskoristiti "errno" ako hocemo vecu kontrolu nad ispisom
        // takodje, recimo ako hocemo negde interno drugacije da belezimo greske
        // i naravno, errno mozemo iskoristiti ako je potrebno obraditi gresku dodatno
        printf("Greska broj:%d %s\n", e, strerror(e));
    }
}