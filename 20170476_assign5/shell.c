#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

#include "exe.h"
#include "shell_signal.h"

int main()
{
    int state = 1;
    char* line;
    char** token_n;
    char*** token_p;
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtstpHandler);
    print_shell();
    while(state)
    {
        print_command();
        line = read_line();
        if(line == NULL) continue;
        state = execute_shell(line, token_n, token_p);
    }
}

