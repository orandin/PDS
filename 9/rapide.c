#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "pile.h"
#include "tri.h"
#include "rapide.h"
#include "main.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
unsigned int nb_threads_work;
pile pile_th;

unsigned long seuil_bloc_long = 512;

base_t *tableau;

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

/* Étape élémentaire du tri rapide : découpe le bloc b en 0, 1 ou 2 blocs
 * Dans le cas normal, découpe en 2 blocs, les éléments inférieurs au
 * pivot, et ceux supérieurs au pivot
 * Si un bloc contient moins de 1 élément, il n’est pas retourné
 */
int rapide_decoupebloc(bloc_t b, bloc_t bret[2]) {
    pos_t g, d;
    base_t pivot, tmp;
    bloc_t b1, b2;
    int nb_ret = 0;

    if(b.debut >= b.fin) {
        /* Arrive uniquement dans le cas d’un tri d’un tableau de
         * taille 1 au départ */
        assert (b.debut == b.fin);
        return 0;
    }

    /* Définit une petite macro pour échanger deux cases de tableau en
     * passant par la variable tmp */
#define echange(p1,p2)                     \
    do {                                   \
        tmp         = tableau[p1];         \
        tableau[p1] = tableau[p2];         \
        tableau[p2] = tmp;                 \
    } while(0)

    pivot = tableau[b.debut];
    g = b.debut + 1;
    d = b.fin;

    while (g < d) {
        while (g < d && tableau[g] <= pivot)
            g++;
        while (d > g && tableau[d] > pivot)
            d--;
        if (g < d)
            echange(g, d);
    }

    b1.debut = b.debut;
    b2.fin = b.fin;

    if (tableau[g] <= pivot) {
        echange(g, b.debut);
        b1.fin   = g - 1;
        b2.debut = min(g + 1, b2.fin);
    } else if (g > b.debut + 1) {
        echange(g - 1, b.debut);
        b1.fin   = max(g - 2, b1.debut);
        b2.debut = g;
    } else {                    /* sinon le pivot est le plus petit, donc déjà bien placé */
        b1.fin   = b.debut;
        b2.debut = b.debut + 1;
    }

    if (b1.debut < b1.fin)
        bret[nb_ret++] = b1;
    if (b2.debut < b2.fin)
        bret[nb_ret++] = b2;

    return nb_ret;
}

/* Effectue un tri rapide séquentiel */
void rapide_seq(bloc_t bloc_init) {
    pile p;
    int i, nb_blocs;
    bloc_t bloc;
    bloc_t blocs[2];

    init_pile(&p);
    empile(&p, bloc_init);

    /* Principe du tri rapide séquentiel :
     * tant qu’il y a des blocs à trier, dépile un bloc, le découpe en
     * (au maximum) deux sous-blocs non-encore triés et les empile */
    do {
        bloc = depile(&p);
        nb_blocs = rapide_decoupebloc(bloc, blocs);
        for (i = 0; i < nb_blocs; i++)
            empile(&p, blocs[i]);
    } while (!pile_vide(&p));
}


void *wrapper(void *arg) {

    int i, nb_blocs;
    bloc_t blocs[2];
    bloc_t bloc;

    while(1) {

        /* Début de la session critique */
        pthread_mutex_lock(&mutex);

        /* Tant que la pile est vide, on attend qu'un autre thread le remplisse. */        
        while(pile_vide(&pile_th) && nb_threads_work > 1) {
            nb_threads_work--;
            pthread_cond_wait(&cond, &mutex);
        }
        
        /* Si la pile est vide, on envoie un signal à tout le monde et on s'arrête. */
        if(pile_vide(&pile_th)) {
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        
        bloc = depile(&pile_th);
        pthread_mutex_unlock(&mutex);
        /* Fin de la session critique */
        
        nb_blocs = rapide_decoupebloc(bloc, blocs);

        /* Début de la session critique */
        pthread_mutex_lock(&mutex);
        
        for(i = 0; i< nb_blocs; i++) {
            if(blocs[i].fin - blocs[i].debut < seuil_bloc_long) {
                rapide_seq(blocs[i]);
                continue;
            }
            empile(&pile_th, blocs[i]);
        }
        pthread_mutex_unlock(&mutex);
        /* Fin de la session critique */
    }
}


void rapide(pos_t taille, unsigned int nb_threads) {
    pthread_t *threads;
    bloc_t bloc;
    int i;

    bloc.debut = 0;
    bloc.fin   = taille - 1;

    if (nb_threads == 1) {
        rapide_seq(bloc);
        return;
    }


    assert(nb_threads > 1);
    init_pile(&pile_th);
    empile(&pile_th, bloc);

    threads = (pthread_t *) malloc(sizeof(pthread_t) * nb_threads);
    assert(threads != NULL);

    for(i = 0; i < nb_threads; i++) {
        nb_threads_work++;
        assert(pthread_create(&threads[i], NULL, wrapper, NULL) == 0);
    }

    for(i = 0; i < nb_threads; i++)
        assert(pthread_join(threads[i], NULL) == 0);

    free(threads);
    return;
}
