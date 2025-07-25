#include<signal.h>

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}

void sigtstpHandler(int sig_num)
{
    signal(SIGTSTP, sigtstpHandler);
    fflush(stdout);
}
