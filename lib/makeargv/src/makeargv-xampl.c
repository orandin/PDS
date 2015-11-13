/* ------------------------------
   $Id: makeargv-main+sol.c,v 1.1 2007/04/30 06:49:22 marquet Exp $
   ------------------------------------------------------------

   Squelette d'utilisation de makeargv

   Philippe Marquet, Apr 2006
   
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "makeargv.h"

int
main (int argc, char *argv[])
{
    int i, status;
    
    for (i=1; i<argc; i++) { /* traiter argv[i] */
        char **cmdargv;
        char **arg;

        /* création du argv de l'argument i */
        status = makeargv(argv[i], " \t", &cmdargv);
        assert(status>0);

        /* test: affichage */
        fprintf(stderr, "[%s]\t%% ", cmdargv[0]);
        for (arg=cmdargv; *arg; arg++)
            fprintf(stderr, "%s ", *arg);
        fprintf(stderr, "\n");

        /* libération mémoire */
        freeargv(cmdargv);
    }

    exit(EXIT_SUCCESS);
}
