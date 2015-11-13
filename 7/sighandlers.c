/* mshell - a job manager */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>


#include "jobs.h"
#include "common.h"
#include "sighandlers.h"

/*
 * wrapper for the sigaction function
 */
int sigaction_wrapper(int signum, handler_t * handler) {
    /*printf("sigaction_wrapper : To be implemented\n");*/

    struct sigaction siga;

    siga.sa_handler = handler;
    siga.sa_flags   = SA_RESTART;
    sigemptyset(&siga.sa_mask);
    sigaction(signum, &siga, NULL);

    return 1;
}

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children
 */
void sigchld_handler(int sig) {
    struct job_t *jobs;
    int status;
    int pid;

    if (verbose)
        printf("sigchld_handler: entering\n");

    while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0 && 1) {

        if (WIFEXITED(status)) {
            jobs_deletejob(pid);
        } else if (WIFSIGNALED(status)) {
            jobs_deletejob(pid);
        } else if (WIFSTOPPED(status)) {
            jobs = jobs_getjobpid(pid);
            jobs->jb_state = ST;
        } else {
            printf("%d\n", pid);
            printf("%d\n", jobs->jb_jid);
        }
    }

    if (verbose)
        printf("sigchld_handler: exiting\n");

    return;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig) {
    int pid;

    if (verbose)
        printf("sigint_handler: entering\n");

    if ((pid = jobs_fgpid()) != 0)
        kill(pid, sig);

    if (verbose)
        printf("sigint_handler: exiting\n");

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig) {
    int pid;

    if (verbose)
        printf("sigtstp_handler: entering\n");
 
    if ((pid = jobs_fgpid()) != 0)
        kill(pid, sig);

    if (verbose)
        printf("sigtstp_handler: exiting\n");

    return;
}
