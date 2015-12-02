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
  unsigned long taille;
  unsigned long res;
};

unsigned long compteur_gc(char *bloc, unsigned long taille) {
    unsigned long i, cptr = 0;
    for (i = 0; i < taille; i++) {
        if (bloc[i] == 'G' || bloc[i] == 'C') {
            cptr++;
        }
    }
    return cptr;
}


void *wrapper(void *arg) {
  struct arg_t *args = (struct arg_t*) arg;
  args->res = compteur_gc(args->bloc, args->taille);
  return NULL;
}


long th_compteur_gc(char *tampon, unsigned long taille, int nb_threads) {

    int i;
    pthread_t *threads;
    struct arg_t **args;
    unsigned long taille_segment;
    unsigned long res = 0;

    threads = (pthread_t*) malloc(sizeof(pthread_t) * nb_threads);
    assert(threads != NULL);

    /* Tableau de pointeurs d'arguments */
    args = (struct arg_t **) malloc(sizeof(struct arg_t *) * nb_threads);
    assert(args != NULL);

    taille_segment = taille / nb_threads;

    /* n-1 car l'argument taille du dernier thread est particulier */
    for (i = 0 ; i < nb_threads - 1; i++) {

        args[i] = (struct arg_t *) malloc(sizeof(struct arg_t));
        assert(args[i] != NULL);
        args[i]->bloc   = &tampon[taille_segment * i];
        args[i]->taille = taille_segment;
        args[i]->res    = 0;
        pthread_create(&(threads[i]), NULL, wrapper, args[i]);
    }
    
    /* Dernier thread */
    args[i] = (struct arg_t *) malloc(sizeof(struct arg_t));
    args[i]->bloc   = &tampon[i * taille_segment];
    args[i]->taille = taille - taille_segment * i;
    args[i]->res    = 0;

    pthread_create(&(threads[i]), NULL, wrapper, args[i]);

    for(i = 0; i < nb_threads; i++){
        pthread_join(threads[i], NULL);
        res += args[i]->res;
        free(args[i]);
    }

    free(threads);
    free(args);

    return res;
}

int main(int argc, char *argv[]) {
    struct stat st;
    int fd;
    char *tampon;
    int lus;
    unsigned long cptr = 0;
    off_t taille = 0;
    struct timespec debut, fin;

    int verbose = 0;
    int nb_threads;

    assert(argv[1] != NULL);
    assert(argv[2] != NULL);

    nb_threads = atoi(argv[2]);
    assert(nb_threads > 0);

    /* Quelle taille ? */
    assert(stat(argv[1], &st) != -1);
    tampon = malloc(st.st_size);
    assert(tampon != NULL);

    /* Chargement en mémoire */
    fd = open(argv[1], O_RDONLY);
    assert(fd != -1);
    while ((lus = read(fd, tampon + taille, st.st_size - taille)) > 0){
        taille += lus;
    }
    assert(lus != -1);
    assert(taille == st.st_size);
    close(fd);

    /* Calcul proprement dit */
    assert(clock_gettime(CLOCK_MONOTONIC, &debut) != -1);
    cptr = th_compteur_gc(tampon, taille, nb_threads);
    assert(clock_gettime(CLOCK_MONOTONIC, &fin) != -1);

    /* Affichage des résultats */
    if (verbose) {
        printf("Nombres de GC:   %ld\n", cptr);
        printf("Taux de GC:      %lf\n", ((double) cptr) / ((double) taille));
    }

    fin.tv_sec  -= debut.tv_sec;
    fin.tv_nsec -= debut.tv_nsec;
    if (fin.tv_nsec < 0) {
        fin.tv_sec--;
        fin.tv_nsec += 1000000000;
    }
    if (verbose) {
        printf("Durée de calcul: %ld.%09ld\n", fin.tv_sec, fin.tv_nsec);
        printf("(Attention: très peu de chiffres après la virgule sont réellement significatifs !)\n");
    } else {
        printf("%d %d %ld.%09ld\n", (int) st.st_size, nb_threads, fin.tv_sec, fin.tv_nsec);
    }
    return 0;
}