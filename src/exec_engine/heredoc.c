

#include "../../inc/minishell.h"
#include <readline/readline.h>

static int	status_check(int status, char *expanded, char *line)
{
	if (status == -1)
	{
		if (expanded)
			free(expanded);
		if (line)
			free(line);
		return (-1);
	}
	return (0);
}

static bool	herdoc_stop(char *line, char *delimiter)
{
	if (!line)
		return (true);
	if (ft_strcmp(line, delimiter) == 0)
		return (true);
	return (false);
}

static int	expanded_herdoc(char *expanded, int write_fd, char *line)
{
	int status;

	status = write(write_fd, expanded, ft_strlen(expanded));
	if (status == -1)
	{
		perror("write failed");
		return (status_check(status, expanded, line));
	}
	status = write(write_fd, "\n", 1);
	if (status == -1)
	{
		perror("write newline failed");
		return (status_check(status, expanded, line));
	}
	printf("[DEBUG] Wrote line: '%s'\n", expanded);
	return (0);
}

int	process_heredoc_input(t_env *env, char *delimiter, int write_fd)
{
	char		*line;
	char		*expanded;
	static bool	first_entry = true;

	printf("[DEBUG] Starting heredoc collection, delimiter: '%s'\n", delimiter);
	printf("[DEBUG] Write fd: %d\n", write_fd);
	
	while (1)
	{
		if (first_entry)
		{
			printf("[DEBUG] FIRST ENTRY - About to call readline for the first time\n");
			first_entry = false;
		}
		printf("[DEBUG] Prompting for input...\n");
		line = readline("> ");
		printf("[DEBUG] Got line: '%s'\n", line ? line : "(null)");
		if (herdoc_stop(line, delimiter))
		{
			if (!line)
				printf("delimited by end-of-file (wanted `%s')\n", delimiter);
			printf("[DEBUG] Heredoc stopped\n");
			free(line);
			break ;
		}
		expanded = expand_var(env, line);
		printf("[DEBUG] Expanded line: '%s'\n", expanded ? expanded : "(null)");
		if (expanded)
		{
			if (expanded_herdoc(expanded, write_fd, line) == -1)
				return (-1);
			free(expanded);
		}
		free(line);
	}
	
	printf("[DEBUG] Successfully completed heredoc collection\n");
	return (0);
}
