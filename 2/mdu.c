/**
 * \file mdu.c
 * \brief TP 2: Parcours de hiérarchie (PDS)
 * \author {Benjamin SZCZAPA, Simon DELBERGHE}
 * \version 1
 * \date Septembre 2015
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>


/* Variables globales
*/
static int opt_follow_links = 0;
static int opt_apparent_size = 0;



/**
 * \struct st_processedFile
 * \brief Structure contenant couple d'identifiant unique pour un fichier
 */
typedef struct st_processedFile st_processedFile;
struct st_processedFile
{
    dev_t st_dev;
    ino_t st_ino;
    st_processedFile *next;
};

/**
 * \struct st_list
 * \brief Structure contenant l'adresse de st_processedFile suivant.
 *
 * st_list consiste a créer un liste chainée avec l'aide de st_processedFile
 * pour éviter le comptage multiple.
 */
typedef struct
{
	st_processedFile *first;
}
st_list;


static st_list *list;
static st_processedFile *file;

/**
 * \fn int valid_subdir(const char *dname)
 * \brief Prototype qui vérifie que le nom du repertoire est différent des valeurs ".", ".."
 * 
 * \return 1 si le sous repertoire est valide, sinon 0.
 */
int valid_subdir(const char *dname) {
	return !(strcmp(".", dname) == 0 || strcmp("..", dname) == 0);
}

/**
 * \fn void addFile(st_list *list, dev_t st_dev, ino_t st_ino)
 * \brief Ajoute les informations "st_dev" et "st_ino" du fichier traitée dans la liste chainée.
 * \param list   Liste chainée contenant les fichiers déjà comptabilisés.
 * \param  st_dev Numéro du périphérique sur lequel se trouve le fichier.
 * \param  st_ino Numéro d'inoeud du fichier.
 */
void addFile(st_list *list, dev_t st_dev, ino_t st_ino) {

	st_processedFile *file = malloc(sizeof(*file));
	
    file->st_dev = st_dev;
    file->st_ino = st_ino;

	file->next  = list->first;
	list->first = file;
}


/**
 * \fn int containsFile(st_list *list, dev_t st_dev, ino_t st_ino)
 * \brief Prototype qui vérifie si le fichier en cours est déjà contenu dans la liste chainée.
 * \param  list   Liste chainée contenant les fichiers déjà comptabilisés.
 * \param  st_dev Numéro du périphérique sur lequel se trouve le fichier.
 * \param  st_ino Numéro d'inoeud du fichier.
 * 
 * \return 1 si le fichier a été trouvé, sinon 0.
 */
int containsFile(st_list *list, dev_t st_dev, ino_t st_ino) {

	st_processedFile *file = list->first;

	while(file != NULL) {
		if (file->st_dev == st_dev && file->st_ino == st_ino)
			return 1;

		file = file->next;
	}
	return 0;
}


/**
 * \fn int du_file (const char *pathname)
 * \param  pathname Chemin d'accès que l'on souhaite connaître la taille
 * \return          [description]
 */
int du_file (const char *pathname) {

	/* DÉCLARATION */
	int size = 0, status;
	struct stat stats;
	struct dirent *subdir;
	DIR *dir;
	char subdir_pathname[PATH_MAX+1];
	
	/* Si on suit les liens, on utilise stat, sinon, lstat */
	status = (opt_follow_links) ? stat(pathname, &stats) : lstat(pathname, &stats);
	assert(status == 0);

	
	/*
		On vérifie si c'est un fichier normal ou un lien et que le fichier
		n'a pas été déjà comptabilisé (couple st_dev et st_ino).
	 */
	
	if ( (S_ISREG(stats.st_mode) || S_ISLNK(stats.st_mode)) 
		&& !containsFile(list, stats.st_dev, stats.st_ino)) { 
		addFile(list, stats.st_dev, stats.st_ino);

		size = (opt_apparent_size) ? stats.st_size : stats.st_blocks;

		/* On vérifie que le fichier est un lien symbolique et que l'on
			suit les liens symbolique et que ce fichier n'est pas déjà
			contenu dans la liste chainée
		*/

		if(S_ISLNK(stats.st_mode) && opt_follow_links) 
		{
			size += readlink(pathname, subdir_pathname, PATH_MAX-1);
			size += du_file(subdir_pathname);
		}

	} else if (S_ISDIR(stats.st_mode)) {
		size = (opt_apparent_size) ? stats.st_size : stats.st_blocks;
		
		/* On ouvre le dossier */
		dir = opendir(pathname);
		assert(dir != NULL);
		/* Pour chaque sous-dossier, on calcule sa taille */
		while( (subdir = readdir(dir)) != NULL ) {
			if (valid_subdir(subdir->d_name) == 1) {
				snprintf(subdir_pathname, PATH_MAX, "%s/%s", pathname, subdir->d_name);
				size += du_file(subdir_pathname);
			}
		}
		printf("%d\t%s\n", size, pathname);
		closedir(dir);
	}
	return size;
}



/* Programme principal
*/
int main(int argc, char *argv[])
{
	int opt, argcmini = 1;
	/* 	Récupération des options :
			-L, -b
	 */
	list = malloc(sizeof(*list));
	file = malloc(sizeof(*file));
	
	while((opt = getopt(argc, argv, "Lb")) != -1) {
		switch(opt) {
			case 'L':
				opt_follow_links = 1;
				argcmini++;
				break;
			case 'b':
				opt_apparent_size = 1;
				argcmini++;
				break;
		}
	}
	if (argc > argcmini) {
		du_file(argv[argc - 1]);
	} else {
		printf("Il manque le pathname.\n");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}