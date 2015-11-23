/* mshell - a job manager */
#include <stdio.h>
#include "pipe.h"
#include "jobs.h"

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>


void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
    
    int *fd = (int *) malloc(sizeof(int) * (nbcmd -1) * 2);
    int i, c;
    pid_t pid;

    assert(pipe(fd) != -1);

    for (i = 0; i < nbcmd; ++i) {
        assert(pipe(&fd[(i*2)]) != -1);
    }


    printf("%s\n", cmds[1][0]);
    printf("%d\n", nbcmd);

    switch(pid = fork()) {
        case -1: 
            exit(EXIT_FAILURE);
        case 0:
            if (nbcmd > 2) {
                for (i = 1; i < nbcmd - 1; i++) {
                    
                    switch(fork()) {
                        case -1: 
                            exit(EXIT_FAILURE);
                        case 0:
                            c = i * 2;
                            dup2(fd[c], STDIN_FILENO);
                            dup2(fd[c+1], STDOUT_FILENO);
                            close(fd[c]);
                            close(fd[c+1]);
                            execvp(cmds[i][0], cmds[i]);
                    }
                }
            }

            switch(fork()) {
                case -1: 
                    exit(EXIT_FAILURE);
                case 0:
                    close(fd[nbcmd-1]);
                    dup2(fd[nbcmd-1], STDIN_FILENO);
                    close(fd[nbcmd - 2]);
                    execvp(cmds[nbcmd-1][0], cmds[nbcmd-1]);
            }

            for (i = 0; i < nbcmd; ++i) {
                close(fd[i]);
            }

            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execvp(cmds[0][0], cmds[0]);
    }

    jobs_addjob(pid, (bg) ? BG : FG, "pipe !!");



    if (!bg) {
        for (i = 0; i < nbcmd; ++i) {
            wait(NULL);
        }
    }

    return;
}
