#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

extern char **environ;

#define MAX_ARGS 100

char *trim_spaces(char *str);
char *read_line(void);
int parse_args(char *line, char **args);
char *find_command(char *cmd);
void execute_cmd(char **args);

char *trim_spaces(char *str)
{
	char *end;

	while (*str && isspace(*str))
		str++;

	if (*str == 0)
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;

	*(end + 1) = '\0';
	return (str);
}

char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (trim_spaces(line));
}

int parse_args(char *line, char **args)
{
	int i = 0;

	args[i] = strtok(line, " ");
	while (args[i] != NULL && i < MAX_ARGS - 1)
	{
		i++;
		args[i] = strtok(NULL, " ");
	}
	return (i);
}

char *find_command(char *cmd)
{
	char *path_env = getenv("PATH");
	char *path_copy, *token;
	char full_path[1024];

	if (!cmd)
		return (NULL);

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (cmd);
		return (NULL);
	}

	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (strdup(full_path));
		}
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

void execute_cmd(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args[0])
		return;

	cmd_path = find_command(args[0]);
	if (!cmd_path)
	{
		write(2, "Command not found\n", 18);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			perror("./hsh");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(&status);

	free(cmd_path);
}

void shell_loop(void)
{
	char *line;
	char *args[MAX_ARGS];
	int argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);

		line = read_line();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(1, "\n", 1);
			break;
		}

		if (*line == '\0')
		{
			free(line);
			continue;
		}

		argc = parse_args(line, args);
		if (argc == 0)
		{
			free(line);
			continue;
		}

		execute_cmd(args);
		free(line);
	}
}

int main(void)
{
	shell_loop();
	return (0);
}
