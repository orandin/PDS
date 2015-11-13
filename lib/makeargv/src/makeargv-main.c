/* ------------------------------
   $Id: makeargv-main.c,v 1.2 2006/04/11 12:01:25 marquet Exp $
   ------------------------------------------------------------

   Squelette d'utilisation de makeargv

   Philippe Marquet, Apr 2006
   
*/

#include <unistd.h>
#include "makeargv.h"

int
main (int argc, char *argv[])
{
    for ( ...i... ) { /* traiter argv[i] */
        char **cmdargv; 
        ...
        makeargv(argv[i], " \t", &cmdargv);
        ...
        execvp(cmdargv[0], cmdargv);
        ...
        freeargv(cmdargv);
        ...
}	
