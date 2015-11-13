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
    int fd[2];
    assert(pipe(fd) != -1);

    switch(fork()) {
    	case -1: exit(EXIT_FAILURE);
    	case 0:
    		close(fd[0]);
    		dup2(fd[1], STDOUT_FILENO);
    		close(fd[1]);
    		execvp(cmds[0][0], cmds[0]);
    }

    switch(fork()) {
    	case -1: exit(EXIT_FAILURE);
    	case 0:
    		close(fd[1]);
    		dup2(fd[0], STDIN_FILENO);
    		close(fd[0]);
    		execvp(cmds[1][0], cmds[1]);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return;
}
