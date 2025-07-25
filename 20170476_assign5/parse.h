#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TOKEN_ARRAY_SIZE 64

char* read_line()
{
    char* line = NULL;
    size_t size = 0;
    if(getline(&line, &size, stdin) == -1)
    {
        printf("No line input\n");
    }
    if(line[0] == '\n') return NULL;
    return line;
}

int check_pipe(char* line)
{
    char* search = strchr(line, '|');
    if(search == NULL) return 0;
    else return 1;
}

static char** make_token(char* line, const char delim[])
{
   int token_array_size = TOKEN_ARRAY_SIZE;
   char** token_array = (char**)calloc(token_array_size, sizeof(char*));
   int index = 0;
   if(!token_array)
   {
       fprintf(stderr, "allocation for token array error!\n");
   }
   char* token;
   token = strtok(line, delim);
   while(token != NULL)
   {
       token_array[index] = token;
       index++;
       // resizing operation
       if(index > TOKEN_ARRAY_SIZE*(0.7))
       {
           char** tmp = token_array;
           int prev_size = token_array_size; 
           token_array_size += TOKEN_ARRAY_SIZE;
           token_array = (char**)realloc(token_array, token_array_size*sizeof(char*));
           // for situation that the original memory change
           if(tmp != token_array)
           {
               for(int i = 0; i < prev_size; i++)
               {
                   token_array[i] = tmp[i];
               }
               for(int i = prev_size; i < token_array_size; i++)
               {
                   token_array[i] = NULL;
               }
           }
           // reallocation fail
           else if(!token_array)
           {
               fprintf(stderr, "reallocation fail for token array\n");
           }

       }

       token = strtok(NULL,delim);
   }
   return token_array;
}

char** make_general_token(char* line)
{
    const char delim[] = " \t\n\r\a";
    return make_token(line, delim);
}


char*** make_pipe_token(char* line)
{
    char*** pipe_command = (char***)calloc(TOKEN_ARRAY_SIZE, sizeof(char**));
    if(!pipe_command)
        fprintf(stderr, "allocation for pipe_command fail\n");
    const char delim1[] = "|";
    const char delim2[] = " \t\n\r\a";
    int index = 0;
    char** flag = make_token(line, delim1);
    while(*flag != NULL)
    {
        pipe_command[index] = make_token(*flag, delim2);
        index++;
        flag++;
    }
    return pipe_command;
}

void free_general_token(char** token)
{
    free(token);
}
void free_pipe_token(char*** token)
{
    char*** flag = token;
    while(*flag  != NULL)
    {
        free(*flag);
        flag++;
    }
    free(token);
}


