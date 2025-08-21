#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * display_prompt - prints the shell prompt
 */
void display_prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

/**
 * read_line - reads a line from stdin
 * Return: pointer to the string read
 */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t nread;

	nread = getline(&line, &bufsize, stdin);
	if (nread == -1)
		return (NULL);

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}

/**
 * split_line - splits a line into arguments
 * @line: input command line
 * Return: array of strings (arguments)
 */
char **split_line(char *line)
{
	char *token;
	char **args;
	int bufsize = 64, position = 0;

	args = malloc(bufsize * sizeof(char *));
	if (!args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " ");
	while (token != NULL)
	{
		args[position++] = token;
		if (position >= bufsize)
		{
			bufsize += 64;
			args = realloc(args, bufsize * sizeof(char *));
			if (!args)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " ");
	}
	args[position] = NULL;
	return (args);
}

/**
 * execute_command - forks and executes a command
 * @line: command line input
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char **argv;

	argv = split_line(line);

	pid = fork();
	if (pid == 0)
	{
		if (execve(argv[0], argv, NULL) == -1)
		{
			perror("./hsh");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
		wait(&status);
	else
		perror("fork");

	free(argv);
}

/**
 * main - simple shell that executes commands with arguments
 * Return: 0 on success
 */
int main(void)
{
	char *line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		execute_command(line);
		free(line);
	}

	return (0);
}
