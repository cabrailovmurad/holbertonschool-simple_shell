#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define MAX_ARGS 100

char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return NULL;
	}
	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';
	return line;
}

char *trim_spaces(char *str)
{
	char *end;

	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	*(end + 1) = '\0';
	return str;
}

char **parse_args(char *line)
{
	char **args = malloc(sizeof(char *) * (MAX_ARGS + 1));
	int i = 0;
	char *token;

	if (!args)
		return NULL;

	token = strtok(line, " \t");
	while (token != NULL && i < MAX_ARGS)
	{
		args[i++] = token;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;
	return args;
}

char *find_command(char *cmd)
{
	char *path = getenv("PATH");
	char *path_copy, *dir;
	char *fullpath = malloc(1024);

	if (!fullpath)
		return NULL;

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return strdup(cmd);
		free(fullpath);
		return NULL;
	}
	if (!path)
	{
		free(fullpath);
		return NULL;
	}
	path_copy = strdup(path);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		snprintf(fullpath, 1024, "%s/%s", dir, cmd);
		if (access(fullpath, X_OK) == 0)
		{
			free(path_copy);
			return fullpath;
		}
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	free(fullpath);
	return NULL;
}

void execute(char **args)
{
	char *cmd_path = find_command(args[0]);
	pid_t pid;
	int status;

	if (!cmd_path)
	{
		write(2, "Command not found\n", 18);
		return;
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, environ);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	free(cmd_path);
}

int main(void)
{
	char *line;
	char **args;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);
		line = read_line();
		if (!line)
			break;
		line = trim_spaces(line);
		if (*line == '\0')
		{
			free(line);
			continue;
		}
		args = parse_args(line);
		if (args[0])
			execute(args);
		free(args);
		free(line);
	}
	return 0;
}
