/**
 * \file exo8.c
 * \brief TP 5: Observation de processus
 * \author Simon DELBERGHE
 * \version 1
 * \date Octobre 2015
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


void race(int nb_fils) {
	int i, status;
	pid_t pid;

	for(i = 0; i < nb_fils; i++) {
		switch(fork()) {
			case -1:
				perror("Erreur fork");
				exit(EXIT_FAILURE);

			/* Fils */
			case 0:
				while(1) {
					printf("%d\n", getpid());
					fflush(stdout);
					sleep(5);
				}
				exit(EXIT_SUCCESS);

		}
	}

	assert(system("ps") != -1);
	i = 1;

	getchar();

	while((pid = wait(&status)) && i <= nb_fils) {

		printf("Le fils [%d] a terminé.\n", pid);
		fflush(stdout);
		i++;
	}
}




int main(int argc, char const *argv[])
{
	race(2);
	return 0;
}