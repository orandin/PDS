/* mshell - a job manager */
#include <stdio.h>
#include <string.h>
#include "pipe.h"
#include "jobs.h"
#include "cmd.h"

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

/* variable qui contiendra la commande */
static char cmdline[1024];

/* reset_signals : remet les signaux à "zéro" */
int reset_signals() {
	struct sigaction sa;

	sa.sa_handler 	= SIG_DFL;
	sa.sa_flags 	= 0;
	assert(sigemptyset(&sa.sa_mask) == 0);

	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGTSTP,&sa,NULL);
	sigaction(SIGCHLD,&sa,NULL);
	
	return 0;
}

/* get_commande : construit la ligne de commande */
void get_commande(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
	int i;
	int j = 0;

	strcpy(cmdline,"");

	for (i = 0 ; i < nbcmd ; i++) {
		while (cmds[i][j]) {
			strcat(cmdline,cmds[i][j]);
			strcat(cmdline," ");
			j++;
		}
		(i != nbcmd - 1) ? strcat(cmdline,"| ") : strcat(cmdline,"");
		j = 0;
	}

	/* Si la commande est en arrière-plan, on rajoute l'esperluette */
	if (bg)
		strcat(cmdline," &");
}


/* do_pipe : exécute une commande composée de pipes */
void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
	
	int fds[MAXCMDS][2];
	pid_t pid;
	sigset_t mask;
	int status;
	int dup_stdin, dup_stdout;
	int i;
	int nb_stopped_commands;
	int nb_cmds_actifs;

	if (verbose) 
		printf("do_pipe: entering\n");

	/* on bloque les signaux */
	assert(sigemptyset(&mask) == 0);
	assert(sigaddset(&mask, SIGINT) == 0);
	assert(sigaddset(&mask, SIGTSTP) == 0);
	assert(sigaddset(&mask, SIGCHLD) == 0);
	assert(sigprocmask(SIG_BLOCK, &mask, NULL) == 0);


	/* copies de l'entrée et de la sortie standards */
	dup_stdout 	= dup(STDOUT_FILENO);
	dup_stdin 	= dup(STDIN_FILENO);

	/* Duplication du mshell */
	switch (pid = fork()) {
		case -1 :
			exit(EXIT_FAILURE);

		case 0 :

			/* On définit le groupe de processus auquel il est attaché */
			setpgid(0,0);

			/* Execution des (n-1) premières commandes */
			for (i = 0 ; i < nbcmd - 1 ; i++) {
				
				pipe(fds[i]);

				switch (pid = fork()) {
					case -1 :
						exit(EXIT_FAILURE);
					case 0 :
						/* on débloque les signaux */
						sigprocmask(SIG_UNBLOCK,&mask,NULL);
						
						close(fds[i][0]);
						dup2(fds[i][1], STDOUT_FILENO);

						if (verbose)
							printf("do_pipe: %s\n", cmds[i][0]);

						/* Execution de la commande */
						execvp(cmds[i][0],cmds[i]);
						exit(EXIT_FAILURE);

					default :
						/* le père redirige son descripteur en lecture sur le tube vers son entrée standard et continue */
						close(fds[i][1]);
						dup2(fds[i][0], STDIN_FILENO);
						break;
				}
			}

			/* Dernière commande */
			
			dup2(dup_stdout, STDOUT_FILENO);

			switch (pid = fork()) {
				case -1 :
					exit(EXIT_FAILURE);

				case 0 :
					/* on débloque les signaux */
					sigprocmask(SIG_UNBLOCK, &mask, NULL);
					
					if (verbose) 
						printf("do_pipe: %s\n", cmds[nbcmd - 1][0]);

					execvp(cmds[nbcmd - 1][0], cmds[nbcmd - 1]);
					exit(EXIT_FAILURE);

				/* Père (fork de mshell) */
				default :
					/* on débloque les signaux */
					sigprocmask(SIG_UNBLOCK, &mask, NULL);

					dup2(dup_stdout, STDOUT_FILENO);
					dup2(dup_stdin, STDIN_FILENO);

					close(fds[nbcmd - 1][0]);
					
					nb_stopped_commands = 0;
					nb_cmds_actifs 		= nbcmd;

					reset_signals();

					/* Tant qu'il reste un fils, on continue */
					while (nb_cmds_actifs > 0) {

						waitpid(-1, &status, WUNTRACED | WCONTINUED);

						if (WIFCONTINUED(status))
							nb_stopped_commands--;

						if ((WIFSIGNALED(status)) || (WIFEXITED(status)))
							nb_cmds_actifs--;

						if (WIFSTOPPED(status))
							nb_stopped_commands++;

						/* Si toutes les commandes ont été stoppées, on stop le fork du mshell */
						if ((nb_stopped_commands == nb_cmds_actifs) && (nb_stopped_commands > 0))
							kill(getpid(), SIGTSTP);
					}

					exit(EXIT_SUCCESS);
			}

		default : 
			
			get_commande(cmds,nbcmd,bg);
			jobs_addjob(pid,(bg ? BG : FG),cmdline);
			sigprocmask(SIG_UNBLOCK,&mask,NULL);

			if (bg != 1)
				waitfg(pid);

			break;
	}

	if (verbose) 
		printf("do_pipe: exiting\n");

	fflush(stdout);
}
