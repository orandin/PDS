#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include <pthread.h>


struct arg_t { 
    char *bloc;
    unsigned long *taille;
    unsigned long res;
};


void * wrapper(void * arg) {

    struct arg_t *a = (struct arg_t *) arg;
    return (void *) compteur_gc(a->bloc, a->taille);
}


unsigned long compteur_gc(char *bloc, unsigned long taille) {
    unsigned long i, cptr = 0;

    for (i = 0; i < taille; i++)
        if (bloc[i] == 'G' || bloc[i] == 'C')
            cptr++;

    return cptr;
}

int main(int argc, char *argv[]) {
    struct stat st;
    int fd;
    char *tampon;
    int lus;
    unsigned long cptr = 0;
    off_t taille = 0;
    struct timespec debut, fin;

    int taille_segment;

    int nbThreads = 0;
    pthread_t threads;


    assert(argv[1] != NULL);
    assert(argv[2] != NULL);

    nbThreads = (int) argv[2];
    taille_segment = taille / nbThreads;


    /* Quelle taille ? */
    assert(stat(argv[1], &st) != -1);
    tampon = malloc(st.st_size);
    assert(tampon != NULL);

    threads = (pthread_t) malloc(sizeof(pthread_t) * nbThreads);

    /* Chargement en mémoire */
    fd = open(argv[1], O_RDONLY);
    assert(fd != -1);
    while ((lus = read(fd, tampon + taille, st.st_size - taille)) > 0)
        taille += lus;
    assert(lus != -1);
    assert(taille == st.st_size);
    close(fd);

   
    /* Calcul proprement dit */
    assert(clock_gettime(CLOCK_MONOTONIC, &debut) != -1);
    /*cptr = compteur_gc(tampon, taille);*/

     for(i = 0; i < nbThreads; i++) {
        struct arg_t *args = (struct arg_t *) malloc(sizeof(struct arg_t *)); 
        args->bloc = tampon + i * taille_segment;
        args->taille = taille_segment; /* Sauf pour le dernier */
        pthread_create(threads[i], NULL, wrapper, args);
    }

    /* Traiter cas du dernier (taille) */
    /* Faire les join */

    assert(clock_gettime(CLOCK_MONOTONIC, &fin) != -1);

    /* Affichage des résultats */
    printf("Nombres de GC:   %ld\n", cptr);
    printf("Taux de GC:      %lf\n", ((double) cptr) / ((double) taille));

    fin.tv_sec  -= debut.tv_sec;
    fin.tv_nsec -= debut.tv_nsec;
    if (fin.tv_nsec < 0) {
        fin.tv_sec--;
        fin.tv_nsec += 1000000000;
    }
    printf("Durée de calcul: %ld.%09ld\n", fin.tv_sec, fin.tv_nsec);
    printf("(Attention: très peu de chiffres après la virgule sont réellement significatifs !)\n");

    return 0;
}
