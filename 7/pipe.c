/* mshell - a job manager */

#include <stdio.h>
#include "pipe.h"

void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
    int fd[2];
    assert(pipe(fd) != -1);

    switch(fork()) {

    	case 0:
    		close(fd[0]);
    		dup2(fd[1], STDOUT_FILENO);
    		close(fd[1]);
    		execvp()
    }

    switch(fork()) {

    	case 0:
    		close(fd[1]);
    		dup2(fd[0], STDINT_FILENO);
    		close(fd[0]);
    		execvp()
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    printf("Not implemented yet\n");

    return;
}
