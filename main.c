#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 100
#define MAX_PATH 1024

/**
 * trim_spaces - remove leading and trailing spaces
 * @str: input string
 * Return: trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;

	if (*str == 0)
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * find_command - search command in PATH
 * @cmd: command name
 * Return: full path if found, else NULL
 */
char *find_command(char *cmd)
{
	char *path_env;
	char *path_copy;
	char *dir;
	static char full_path[MAX_PATH];

	if (!cmd)
		return (NULL);

	if (strchr(cmd, '/'))
		return (cmd);

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		snprintf(full_path, MAX_PATH, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * main - simple shell loop
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;
	char *args[MAX_ARGS];
	int i;
	char *cmd_path;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, ":) ", 3);

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
		if (*line == '\0')
			continue;

		args[0] = strtok(line, " ");
		if (args[0] == NULL)
			continue;

		for (i = 1; i < MAX_ARGS; i++)
		{
			args[i] = strtok(NULL, " ");
			if (args[i] == NULL)
				break;
		}

		cmd_path = find_command(args[0]);
		if (!cmd_path)
		{
			write(2, "Command not found\n", 18);
			continue;
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
			execve(cmd_path, args, environ);
			perror("./hsh");
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
		}
	}

	free(line);
	return (0);
}
