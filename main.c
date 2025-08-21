#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

extern char **environ;

#define MAX_ARGS 100

/* Function to remove leading and trailing spaces */
char *trim_spaces(char *str)
{
    char *end;

    while (*str && isspace(*str))  /* skip leading spaces */
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))  /* remove trailing spaces */
        end--;

    *(end + 1) = '\0';
    return str;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status;
    char *args[MAX_ARGS];
    int i;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(1, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            if (isatty(STDIN_FILENO))
                write(1, "\n", 1);
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        line = trim_spaces(line);

        if (*line == '\0')  /* empty line */
            continue;

        /* split the line into arguments */
        args[0] = strtok(line, " ");
        if (args[0] == NULL)
            continue;

        for (i = 1; i < MAX_ARGS; i++)
        {
            args[i] = strtok(NULL, " ");
            if (args[i] == NULL)
                break;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(line);
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (execve(args[0], args, environ) == -1)
            {
                perror("./hsh");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(&status);
        }
    }

    free(line);
    return 0;
}
