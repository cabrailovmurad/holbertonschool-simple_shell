#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

char **splitter(char *str, const char *delim);

char **pathfinder(void)
{
	int j = 0;
	char *temp = NULL;
	char **array = NULL;

	while (environ[j])
	{
		array = splitter(environ[j], "=");
		if (array && !strcmp(array[0], "PATH"))
		{
			if (array[1])
				temp = strdup(array[1]);
			for (int i = 0; i < 2; i++)
				free(array[i]);
			free(array);
			break;
		}
		if (array)
		{
			for (int i = 0; i < 2; i++)
				free(array[i]);
			free(array);
		}
		j++;
	}

	if (!temp)
		return NULL;

	array = splitter(temp, ":");
	for (int i = 0; array[i]; i++)
	{
		char *tmp = malloc(strlen(array[i]) + 2);
		strcpy(tmp, array[i]);
		strcat(tmp, "/");
		free(array[i]);
		array[i] = tmp;
	}

	free(temp);
	return array;
}
