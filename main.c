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

