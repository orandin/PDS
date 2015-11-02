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


int
valeurRetour(int actual, int newValue) {

	/* -- AND -- */
	if (conjonction)
		return actual && newValue;
	/* -- OR -- */
	else 
		return actual || newValue;
}



int 
mdo(int debut, int argc, char *argv[]) {

	int status;
	int retour = 1;
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
			if (WEXITSTATUS(status)) {
				retour = valeurRetour(retour, 0);
			}

		} else {
			retour = valeurRetour(retour, 0);
		}


		printf("%d a terminé.\n", pid);
		i--;
	}

	return !(retour);
}


int 
main(int argc, char *argv[])
{
	int opt;
	int debut = 1;
	
	while((opt = getopt(argc, argv, "ao")) != -1) {
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