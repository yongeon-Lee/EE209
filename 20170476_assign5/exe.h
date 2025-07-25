#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<string.h>

#include "parse.h"
#define PATH_MAX 1024
enum { FALSE, TRUE };

void print_shell()
{
    printf("----------------------------------------\n");
    printf("\tAssignment #3 - swsh\n");
    printf( "----------------------------------------\n");
    printf("     Welcome to Mini Shell Program\n");
    printf("     Copyright(c) KAIST EE 17 LEE\n");
    printf( "----------------------------------------\n");
}

void print_command()
{
    char path[PATH_MAX];
    if(getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s:~$ ", path);
    }
}

int execute_pipe_command(char*** token)
{
    int pipefd[2];
    pid_t pid;
    int prev;
    while(*token != NULL)
    {
        pipe(pipefd);
        pid = fork();
        if(pid == -1)
            exit(EXIT_FAILURE);
        else if(pid == 0)
        {
           dup2(prev, STDIN_FILENO);
           if(*(token + 1) != NULL)
           {
               dup2(pipefd[1], STDOUT_FILENO);
           }
           close(pipefd[0]);
           if(execvp(*(token)[0], *token) == -1)
           {
               perror("Error");
           }
           exit(EXIT_FAILURE);
           return FALSE;
        }
        else
        {
            wait(NULL);
            close(pipefd[1]);
            prev = pipefd[0];
            token++;
        }
    }
    return TRUE;
}

int execute_command(char** token)
{
    /* first make built-in command */
    /* case 1: cd command */
    if(!strcmp(token[0], "cd"))
    {
        if(token[2] != NULL)
        {
            perror("./swsh: cd takes only one parameter\n");
        }
        if(token[1] == NULL)
        {
            chdir(getenv("HOME"));
        }
        if(chdir(token[1]) == -1)
        {
            perror("./swsh: No such file or directory\n");
        }
        return TRUE;
    }
    /* case 2: exit command */
    if(!strcmp(token[0], "exit"))
    {
        if(token[1] != NULL)
        {
            perror("./swsh: exit does not take parameters\n");
            return TRUE;
        }
        free_general_token(token);
        exit(EXIT_FAILURE);
    }

    /* Not built-in command */
    pid_t pid, wpid;
    int status;
    pid = fork();

    if(pid == -1) perror("fork fail");
    else if(pid == 0)
    {
        if(execvp(token[0], token) == -1)
        {
            perror("error");
        }
        exit(EXIT_FAILURE);
    }
    do
    {
        wpid = waitpid(pid, &status, WUNTRACED); 
    }while(!WIFEXITED(status) && !WIFSIGNALED(status));

    return TRUE;
}


int execute_shell(char* line, char** token_n, char*** token_p)
{
    int state;
    if(check_pipe(line) == FALSE)
    {
        token_n = make_general_token(line);
        state = execute_command(token_n);
        free_general_token(token_n);
    }
    else
    {
        token_p = make_pipe_token(line);
        state = execute_pipe_command(token_p);
        free_pipe_token(token_p);
    }
    return state;
}
