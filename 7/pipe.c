/* mshell - a job manager */
#include <stdio.h>
#include "pipe.h"

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>


void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
    
    int fd = malloc(sizeof(int) * (nbcmd -1) * 2);

    int i;
    assert(pipe(fd) != -1);

    for (int i = 0; i < nbcmd; ++i) {
        assert(pipe(fd[i]) != -1);
    }


    printf("%s\n", cmds[1][0]);

    switch(fork()) {
        case -1: 
            exit(EXIT_FAILURE);
        case 0:
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execvp(cmds[0][0], cmds[0]);
    }

    if (nbcmd > 2) {
        for (i = 1; i < nbcmd; i++) {
            
            switch(fork()) {
                case -1: 
                    exit(EXIT_FAILURE);
                case 0:
                    dup2(fd[0], STDIN_FILENO);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    execvp(cmds[i][0], cmds[i]);
            }
        }
    }

    switch(fork()) {
    	case -1: 
            exit(EXIT_FAILURE);
    	case 0:
    		close(fd[1]);
    		dup2(fd[0], STDIN_FILENO);
    		close(fd[0]);
    		execvp(cmds[nbcmd][0], cmds[nbcmd]);
    }

    close(fd[0]);
    close(fd[1]);


    if (!bg) {
        for (i = 0; i < nbcmd; ++i) {
            wait(NULL);
        }
    }

    return;
}
