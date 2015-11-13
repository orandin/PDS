/**
 * \file mtail.c
 * \brief TP 3: Afficher la fin d’un fichier (PDS)
 * \author Benjamin SZCZAPA, Simon DELBERGHE
 * \version 1
 * \date Octobre 2015
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>


/* Préprocesseur */
#define ESPACE_TAMPON 512

/**
 * \fn int 	compteurLigne(int fd)
 * \brief 	Prototype qui se charge de compter le nombre de lignes d'un fichier.
 * 			Le compteur commence à partir de 1. C'est-à-dire que pour un fichier
 * 			vide, il a une ligne.
 *
 * \param int Descripteur de fichier
 * \param int Le nombre de lignes à afficher
 * \return 	Le nombre de lignes N du fichier (avec N >= 1)
 */
int compteurLigne(int fd, int *ntail) {
	int i, lus, ligneVide;
	int nbLigne = 1;
	int count   = ESPACE_TAMPON;
	char tampon[ESPACE_TAMPON];

	while((lus = read(fd, tampon, count)) > 0) {
		/*  Si on lit quelque chose, cela sous-entend que la ligne contient au moins un
			caractère. Il ne s'agit pas d'une ligne vide. */
		ligneVide = 0;

		for (i = 0; i < lus; i++) {
			if ((char) tampon[i] == '\n') {
				nbLigne++;

				/* 	Si l'on se trouve à la fin du tampon, on enregistre qu'il existe,
					pour le moment, une ligne vide. */
				if (i + 1 >= lus)
					ligneVide = 1;
			}
		}
    }

    /* S'il existe une ligne vide, une ajoute 1 à ntail. */
    if (ligneVide)
    	*ntail += 1;

    return nbLigne;
}


/*
Si 50 lignes et l'on souhaite lire juste les 10 (n) dernières lignes,
alors on commence à afficher à partir de 50 - n lignes
*/

int main(int argc, char *argv[])
{
	int opt, fd, i, nbLigne, lus;
	int ntail = 10;
	int count = ESPACE_TAMPON;
	char tampon[ESPACE_TAMPON];

	/* Récupération de l'argument de l'option n, si elle existe. */
	opt = getopt(argc, argv, "n:");
	switch(opt) {
		case 'n':
			ntail =  atoi(optarg);
			break;
	}

	/* Première ouverture pour comptage des lignes */
	fd = open(argv[argc-1], O_RDONLY);
	assert(fd != -1);

	nbLigne = compteurLigne(fd, &ntail);
	assert(close(fd) == 0);

	/* Seconde ouverture pour l'affichage des ntail lignes */
	fd = open(argv[argc-1], O_RDONLY);
	assert(fd != -1);


	/* 	Si le nombre de lignes du fichier est inférieur, au ntail
		on affiche tout.  */
	if (nbLigne < ntail) {
		while((lus = read(fd, tampon, count)) > 0) {
			for (i = 0; i <  lus; i++) {
				printf("%c", (char) tampon[i]);
			}
		}
	/* Sinon, on affiche seulement à partir des ntail dernières lignes. */
	} else {
		nbLigne -= ntail;

		while((lus = read(fd, tampon, count)) > 0) {
			for (i = 0; i <  lus; i++) {
				if (nbLigne <= 0) {
					printf("%c", (char) tampon[i]);
				} else if ((char) tampon[i] == '\n') {
					nbLigne--;
				}
			}
		}
	}
	assert(close(fd) == 0);

	return 0;
}
