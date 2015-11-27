/**
 * \file do.c
 * \brief TP 6: Mutation de processus (PDS)
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
#include <string.h>

#include "makeargv.h"


static int conjonction = 1;
static int opt_cc = 0;
static int opt_k = 0;

/**
 * \fn int 	valeurRetour(int actual, int newValue)
 * \brief 	Prototype qui détermine la valeur de retour du programme 
 * 			en fonction de la valeur précédente.
 *
 * \param actual 	Valeur de retour actuelle
 * \param newValue 	Nouvelle valeur de retour
 */
int
valeurRetour(int actual, int newValue) {

	/* -- AND -- */
	if (conjonction)
		return actual && newValue;
	/* -- OR -- */
	else 
		return actual || newValue;
}



/**
 * \fn int 	mdo(int debut, int argc, char *argv[]) 
 * \brief 	Prototype qui exécute un ensemble de commandes
 *
 * \param debut Index définissant la position de départ pour la lecture des arguments
 * \param argc 	Nombre d'arguments
 * \param argv 	Arguments
 */
int 
mdo(int debut, int argc, char *argv[]) {

	int status;
	int retour = conjonction;
	int i, pid;
	
	for (i = debut; i < argc; i++) {
        char **cmdargv;

        /* création du argv de l'argument i */
		assert(makeargv(argv[i], " \t", &cmdargv) > 0);

        switch(fork()) {
			case -1: 
				perror("Fork");
				exit(EXIT_FAILURE);
				break;
			case 0:
				execvp(cmdargv[0], cmdargv);
				perror("Execvp");
				exit(EXIT_FAILURE);
		}

        freeargv(cmdargv);
    }



	while ((pid = wait(&status)) != -1 && i > 0) {

		if (WIFEXITED(status)) {
			retour = valeurRetour(retour, (WEXITSTATUS(status) == 0));
		} else {
			retour = valeurRetour(retour, 0);
		}

		/*
		 * Court-circuit
		 *
		 * Si l'option est activé, on vérifie que dans le cas d'un :
		 * OR, la valeur de retour vaut TRUE
		 * AND, la valeur de retour vaut FALSE
		 *
		 * Si les conditions sont remplies, on retourne la valeur
		 */
		if (opt_cc && ((!retour && conjonction) || (!conjonction && retour))) {
			
			/* Si TRUE, on tue tous les processus du groupe correspondant au pid du père */
			if (opt_k)
				killpg(getpid(), SIGINT); 

			return !(retour);
		}

		i--;
	}

	return !(retour);
}

/* Main */
int 
main(int argc, char *argv[])
{
	int opt;
	int debut = 1;
	
	while((opt = getopt(argc, argv, "aock")) != -1) {
		switch(opt) {
			case 'o':
				conjonction = 0;
				break;

			case 'c':
				opt_cc = 1;
				break;

			case 'k':
				opt_k = 1;
				break;
		}
		debut++;
	}
    return mdo(debut, argc, argv);
}