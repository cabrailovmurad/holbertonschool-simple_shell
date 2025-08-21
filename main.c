#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 1024

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *args[100];
    int i;

    while (1)
    {
        write(1, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        i = 0;
        args[i] = strtok(line, " ");
        while (args[i] != NULL)
            args[++i] = strtok(NULL, " ");

        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
            break;

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        if (pid == 0)
        {
            execvp(args[0], args);
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
        }
    }

    free(line);
    return 0;
}
