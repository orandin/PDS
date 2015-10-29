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

static int conjonction = 1;


int mdo(int debut, int argc, char *argv[]) {

	int status;
	int retour = 1;
	int i, pid;

	for(i = argc; i >= debut; i--) {
		switch(fork()) {
			case -1: 
				perror("Fork");
				exit(EXIT_FAILURE);
				break;
			case 0:
				execvp(argv[i], argv + i);
				perror("Execvp");
				exit(EXIT_FAILURE);

		}

		argv[i] = NULL;
	}

	while ((pid = wait(&status)) != -1 && i > 0) {

		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status))
				retour = 0;

		} else {
			retour = 0;
		}


		printf("%d a terminé.\n", pid);
		i--;
	}

	return retour;
}


int main(int argc, char *argv[])
{
	int i, opt;
	int debut = 1;

	while((opt = getopt(argc, argv, "ao")) != -1) {
		switch(opt) {
			case 'o':
				conjonction = 0;
				
			default:
				debut++;
		}
	}


	/* DEBUG */

	fprintf(stderr, "argc = %d\n", argc);
	
	for (i = 0; i < argc; i++) {
		fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
	}
	/* FIN DEBUG */

	mdo(debut, argc, argv);
	return 0;
}