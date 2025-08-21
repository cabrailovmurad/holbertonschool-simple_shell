#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;
#define MAX_ARGS 100
#define MAX_PATH 1024

char *trim_spaces(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    *(end + 1) = '\0';
    return str;
}

char *find_command(char *cmd)
{
    char *path_env;
    char *path_copy;
    char *dir;
    static char full_path[MAX_PATH];

    if (!cmd)
        return NULL;

    if (strchr(cmd, '/'))
        return cmd;

    path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        snprintf(full_path, MAX_PATH, "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main(void)
{
    char *line;
    size_t len;
    ssize_t nread;
    pid_t pid;
    int status;
    char *args[MAX_ARGS];
    int i;
    char *cmd_path;

    line = NULL;
    l
