/* mshell - a job manager */
#include <stdio.h>
#include <string.h>
#include "pipe.h"
#include "jobs.h"

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

/* variable qui contiendra la commande */
static char cmdline[1024];

/* build_cmdline : construit la ligne de commande */
void build_cmdline(char *cmds[MAXCMDS][MAXARGS],int nbcmd,int bg) {
    int i;
    int j = 0;

    strcpy(cmdline, "");

    for (i = 0 ; i < nbcmd ; i++) {
        while (cmds[i][j]) {
            strcat(cmdline, cmds[i][j]);
            strcat(cmdline, " ");
            j++;
        }
        (i != nbcmd - 1) ? strcat(cmdline,"| ") : strcat(cmdline,"");
        j = 0;
    }

    /* quand la commande est en arrière-plan, on rajoute l'esperluette à sa fin */
    (bg) ? strcat(cmdline," &") : 0;
}


void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
    
    int *fd = (int *) malloc(sizeof(int) * (nbcmd -1) * 2);
    /*int fd[MAXCMDS][2];*/
    int i, c;
    pid_t pid;

    assert(pipe(fd) != -1);

    build_cmdline(cmds, nbcmd, bg);

    for (i = 0; i < nbcmd; ++i) {
        assert(pipe(&fd[(i*2)]) != -1);
    }

    switch(pid = fork()) {
        case -1: 
            exit(EXIT_FAILURE);
        case 0:

            setpgid(0,0);

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

    jobs_addjob(pid, (bg) ? BG : FG, cmdline);



    if (!bg) {
        for (i = 0; i < nbcmd; ++i) {
            wait(NULL);
        }
    }

    free(fd);

    return;
}
