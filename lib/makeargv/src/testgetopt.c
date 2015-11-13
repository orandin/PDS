/* ------------------------------
   $Id: testgetopt.c,v 1.1 2012/01/10 14:34:03 marquet Exp $
   ------------------------------------------------------------

   Illustration de l'utlisation de getopt()
   Philippe Marquet, Jan 2012

   http://www.gnu.org/software/libc/manual/html_node/Getopt.html
   
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *cmdname;

static void
usage ()
{
    fprintf(stderr, "usage: %s [options]... [args]...\n", cmdname);    
    fprintf(stderr, "\toptions : -a -b -c name -d name\n");
    exit(EXIT_FAILURE);
}

static void
unknown_opt (char opt)
{
    if (isprint(opt))
        fprintf(stderr, "Unknown option `-%c'.\n", opt);
    else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", opt);

    usage();
}

int
main (int argc, char **argv)
{
    int aflag = 0, bflag = 0;
    char *cvalue = NULL, *dvalue = NULL;
    int index, c;

    cmdname = argv[0];
    opterr = 0;
    
    while ((c = getopt(argc, argv, "abc:d:")) != -1) {
        switch (c) {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = 1;
                break;
            case 'c':
                cvalue = optarg;
                break;
            case 'd':
                dvalue = optarg;
                break;
            case '?':           /* missing option argument */
                if (optopt == 'c' || optopt == 'd') {
                    fprintf(stderr,
                            "Option -%c requires an argument.\n", optopt);
                    usage();
                } else
                    unknown_opt(optopt);
            default:
                unknown_opt(c);
        }
    }
    
    printf("aflag = %d, bflag = %d, cvalue = %s, dvalue = %s\n",
           aflag, bflag, cvalue, dvalue);
    
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
    exit(EXIT_SUCCESS);
}
