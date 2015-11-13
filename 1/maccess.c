#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	
	/* Déclarations des variables
	*/
	short opt_verbose = 0;
	short opt_read    = 0;
	short opt_write   = 0;
	short opt_execute = 0; 
	int mode          = 0;
	int i, result;


	/* 	Récupération des options :
			-r, -w, -x, -v
	 */
	for (i = 1; i < argc; i++)	{
		if (argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'r':
					opt_read = 1;
					break;
				case 'w':
					opt_write = 1;
					break;
				case 'x': 
					opt_execute = 1;
					break;
				case 'v':
					opt_verbose = 1;
					break;
			}
		}
	}

	/* 	On vérifie qu'au moins une option de vérification d'accès a été renseignée.
		Sinon, on arrête le programme. 
	*/
	
	if (opt_read || opt_write || opt_execute) {
		if (opt_read) mode    += R_OK;
		if (opt_write) mode   += W_OK;
		if (opt_execute) mode += X_OK;
	} else {
		printf("Aucune option de vérification d'accès n'a été renseignée.\n");
		exit(EXIT_FAILURE);
	}


	/* 	On exécute la fonction access();
	 */
	result = access(argv[argc -1], mode);
	
	
	/* 	Si le résultat de retour de access est -1, alors on quitte le programme
		avec une erreur.
	*/
	if (result == -1) {


		/* 	Si l'option -v (verbose) est activé, alors on fournit une explication 
			à l'erreur rencontrée (errno).
		*/
		
		if (opt_verbose) {
			switch (errno) {
				case EACCES:
					printf("Accès refusé.\n");
					break;
				case ELOOP:
					printf("Trop de liens symboliques ont été rencontrés.\n");
					break;
				case ENAMETOOLONG:
					printf("Nom de fichier trop long.\n");
					break;
				case ENOENT:
					printf("Un élément du chemin d'accès (pathname) n'existe pas ou est un pointeur pointant nulle part.\n");
					break;
				case ENOTDIR:
					printf("Un élément du chemin d'accès (pathname) n'est pas un repertoire.\n");
					break;
				case EROFS:
					printf("Il n'est pas permis de demander une écriture sur un système de fichiers en lecture seule.\n");
					break;
				default:
					perror("");
			}
		}

		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
