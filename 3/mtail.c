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
#define ESPACE_TAMPON 2

/**
 * \fn int 	index_tail_buffer(const char *buffer, int bufsize, int ntail, int *nlines)
 * \brief 	Prototype qui retourne l'index du début des ntail dernières lignes. Cet
 * 			index est relatif au tampon de taille bufsize. Si le tampon comporte moins
 * 			de ntail lignes, une valeur négative est retournée et nlines est positionné
 * 			avec le nombre de lignes du buffer. 
 *
 * \param buffer 	Tampon
 * \param bufsize 	Taille du tampon
 * \param ntail 	Nombre de lignes à afficher
 * \param nlines 	Nombre de lignes du tampon (buffer)
 */
int index_tail_buffer(const char *buffer, int bufsize, int ntail, int *nlines) {
	
	int i;
	for (i = 0; i < bufsize; i++) {
		if ((char) buffer[i] == '\n') {
			*nlines += 1;

			if (ntail == *nlines)
				return i;
		}
	}
	return -1;
}

/**
 * \fn void tail_before_pos(int fd, unsigned int pos, int ntail)
 * \brief 	Fonction qui considère le contenu du fichier référencé par le 
 *			descripteur fd jusqu'à sa position pos. Cette position pos est
 *			comprise comme un déplacement depuis la fin du fichier. Cette 
 *			fonction récursive affiche les ntail dernières lignes du fichier.
 *
 * \param fd 	Descripteur de fichier
 * \param pos 	Position du fichier
 * \param ntail Nombre de lignes à afficher
 */
void tail_before_pos(int fd, unsigned int pos, int ntail) {
	
	char tampon[ESPACE_TAMPON];
	int count = ESPACE_TAMPON;
	int nbligne = 0, index, i;
    int curseur = -(ESPACE_TAMPON + pos);

	lseek(fd, curseur, SEEK_END);

	count = read(fd, tampon, ESPACE_TAMPON);
	index = index_tail_buffer(tampon, count, ntail, &nbligne);

	if(index == -1){
		tail_before_pos(fd, pos - count, ntail - nbligne);
		for (i = 0; i < count; i++)	{
			printf("%c", (char) tampon[i]);
		}
	} else {
		for (i = 0; i < index; i++)	{
			printf("%c", (char) tampon[i]);
		}
	}
}


/**
 * \fn void tail(const char *path, int ntail);
 * \brief 	Prototype qui affiche les ntail dernières lignes du fichier
 * 			désigné.
 *
 * \param path 	Chemin du fichier
 * \param ntail Nombre de lignes à afficher
 */
void tail(const char *path, int ntail) {
	int fd;

	fd = open(path, O_RDONLY);
	assert(fd != -1);
	tail_before_pos(fd, 0, ntail);
	assert(close(fd) == 0);
}


/*	Main  */

int main(int argc, char *argv[])
{
	int opt, nbLigne = 10;

	opt = getopt(argc, argv, "n:");
	switch(opt) {
		case 'n':
			nbLigne =  atoi(optarg);
			break;
	}

	tail(argv[argc - 1], nbLigne);
	return 0;
}