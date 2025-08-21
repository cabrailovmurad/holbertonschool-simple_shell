#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
 * execute_command - forks and executes a command
 * @line: command to execute
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char *argv[2];

	argv[0] = line;
	argv[1] = NULL;

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
}

/**
 * main - simple shell that executes one command at a time
 * Return: 0 on success
 */
int main(void)
{
	char *line;

	while (1)
	{
		display_prompt();
		line = read_line();
		if (line == NULL)
		{
			printf("\n");
			break;
		}

		execute_command(line);
		free(line);
	}

	return (0);
}
