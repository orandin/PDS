/**
 * \file race.c
 * \brief TP 5:  À vos marques, prêts...
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
#include <sys/wait.h>

/**
 * \fn void compteur() 
 * \brief 	Compte jusqu'à 100 000 000. Affiche un message et compte jusqu'à 100 000 000
 */
void compteur() {
	int i, count = 100000000;
	for (i = 0; i < count; i++);
	printf("Fils [%d]: J'ai fini de compter jusqu'à %d!\n", getpid(), count);
	fflush(stdout);
	for (i = 0; i < count; i++);
}

/**
 * \fn void race() 
 * \brief 	Créer N fils qui exécutent la fonction compteur et affiche leur ordre d'arrivé.
 */
void race() {
	int i, status, nb_fils = 10;
	pid_t pid;

	for(i = 0; i < nb_fils; i++) {
		pid = fork();
		switch(pid) {
			case -1:
				perror("Erreur fork");
				exit(EXIT_FAILURE);

			/* Fils */
			case 0:
				compteur();
				exit(EXIT_SUCCESS);

			default:
				printf("Fils %d crée !\n", pid);
				fflush(stdout);
		}
	}
	i = 1;

	while((pid = wait(&status)) && i <= nb_fils) {

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			printf("%d: le fils [%d]\n", i, pid);
		} else {
			printf("%d: le fils [%d] mais ne s'est pas terminé correctement\n", i, pid);
		}
		fflush(stdout);
		i++;
	}
}

/**
 * \fn int 	main(int argc, char const *argv[])
 * \brief 	Méthode principale qui appelle la méthode multif()
 *
 * \param argc Nombre d'arguments 
 * \param argv Tableau d'arguments
 * \return L'état d'exécution du programme.
 */
int main(int argc, char const *argv[])
{
	race();
	return 0;
}