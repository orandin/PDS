/**
 * \file mcat-scd.c
 * \brief TP 4:  Performances des entrées/sorties (PDS)
 * \author Simon DELBERGHE
 * \version 1
 * \date Octobre 2015
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


/**
 * \fn int 	int main(int argc, char const *argv[])
 * \brief 	Prototype qui se charge de compter le nombre de lignes d'un fichier.
 * 			Le compteur commence à partir de 1. C'est-à-dire que pour un fichier
 * 			vide, il a une ligne.
 *
 * \param argc Nombre d'arguments
 * \param *argv[] Pointeur vers les arguments
 * \return L'état de l'exécution du programme
 */
int main(int argc, char const *argv[])
{
	int fd, lus, bufsize;
	char *tampon = NULL;
	
	if (getenv("MCAT_BUFSIZ") == NULL) {
		printf("Erreur: La variable d'environnement MCAT_BUFSIZ n'a pas été initialisée.\n");
		fflush(stdout);
		exit(EXIT_FAILURE);
	}

	bufsize = atoi(getenv("MCAT_BUFSIZ"));
	tampon  = malloc(bufsize * sizeof(char));

	fd = open(argv[argc-1], O_RDONLY);
	assert(fd != -1);

	while((lus = read(fd, tampon, bufsize)) > 0) {
		assert(write(STDOUT_FILENO, tampon, lus) == lus);
	}

	assert(close(fd) == 0);
	return 0;
}