/**
 * \file multif.c
 * \brief TP 5:  Performances des entrées/sorties (PDS)
 * \author Simon DELBERGHE
 * \version 1
 * \date Octobre 2015
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Déclaration d'un nouveau type */
typedef int (*func_t)(int);


/**
 * \fn int 	test(int a)
 * \brief 	Fonction qui retourne vrai quand le paramètre a est supérieur à 10.
 * 			Sinon, elle retourne faux.
 *
 * \param int Un nombre/chiffre A
 * \return 	Vrai si le paramètre A est supérieur à 10, sinon faux
 */
int test(int a) {
	printf("Appel de f (%d)\n", a);
	fflush(stdout);
	return (a > 10);
}


/**
 * \fn int 	multif(func_t f[], int args[], int n)
 * \brief 	Prototype qui se charge de compter le nombre de lignes d'un fichier.
 * 			Le compteur commence à partir de 1. C'est-à-dire que pour un fichier
 * 			vide, il a une ligne.
 *
 * \param f[] 		Tableau de fonctions
 * \param args[] 	Tableau d'arguments des fonctions
 * \param n 		Le nombre de fonctions dans f[]
 * \return 	La composition des valeurs de retour des fonctions exécutées
 */
int multif(func_t f[], int args[], int n) {
	
	int i, status, retour = 1;

	for(i=0; i < n; i++) {
		switch(fork()) {
			case -1:
				perror("Erreur fork");
				exit(EXIT_FAILURE);

			/* Fils */
			case 0:
				if (f[i](args[i]))
					exit(EXIT_SUCCESS);
				else
					exit(EXIT_FAILURE);
		}
	}

	/* Père qui attend chaque fils. */
	while(wait(&status) && i > 0) {

		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status))
				retour = 0;

		} else {
			retour = 0;
		}
		i--;
	}

	return retour;
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
	int n, i, *arg, res;
	func_t *f;

	n = atoi(argv[1]);

	f = malloc(n * sizeof(func_t));
	arg = malloc(n * sizeof(int));

	for (i = 0; i < n; i++) {
		f[i] = test;
		arg[i] = i;
	}

	res = multif(f, arg, n);
	printf("Valeur de retour (multif): %d\n", res);
	
	if (res == 1)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}