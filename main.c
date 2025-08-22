#include "shell.h"

/**
 * get_path_value - get PATH from environ without using getenv
 *
 * Return: pointer to PATH string or NULL
 */
char *get_path_value(void)
{
    int i = 0;
    size_t len = 5;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", len) == 0)
            return (environ[i] + len);
        i++;
    }
    return (NULL);
}

/**
 * find_command - search for command in PATH
 * @cmd: command to find
 *
 * Return: full path or NULL
 */
char *find_command(char *cmd)
{
    char *path, *path_copy, *dir, *full_path;
    size_t len;

    path = get_path_value();
    if (!path)
        return (NULL);

    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        len = strlen(dir) + strlen(cmd) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return (NULL);
        }
        snprintf(full_path, len, "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}

/**
 * main - simple shell
 *
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL, *args[64], *token, *cmd_path;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    int status, i;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            exit(0);
        }
        line[read - 1] = '\0';

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            exit(0);
        }

        i = 0;
        token = strtok(line, " ");
        while (token != NULL && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        if (access(args[0], X_OK) != 0)
        {
            cmd_path = find_command(args[0]);
            if (cmd_path)
                args[0] = cmd_path;
            else
            {
                fprintf(stderr, "%s: command not found\n", args[0]);
                continue;
            }
        }

        pid = fork();
        if (pid == 0)
        {
            if (execve(args[0], args, environ) == -1)
                perror("execve");
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork");
        }
    }
    free(line);
    return (0);
}
