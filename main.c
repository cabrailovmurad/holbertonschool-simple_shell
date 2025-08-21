#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

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

        char *cmd = strtok(line, " ");
        while (cmd != NULL)
        {
            pid_t pid = fork();
            int status;

            if (pid == -1)
            {
                perror("fork");
                free(line);
                exit(EXIT_FAILURE);
            }
            if (pid == 0)
            {
                char *argv[] = {cmd, NULL};
                if (execve(argv[0], argv, environ) == -1)
                {
                    perror("./hsh");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                wait(&status);
            }

            cmd = strtok(NULL, " ");
        }
    }

    free(line);
    return 0;
}
