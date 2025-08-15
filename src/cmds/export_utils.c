/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:05:15 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:25:14 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_format_line(char *env_line, char *equal_sign, size_t key_len)
{
	char	*formatted;

	formatted = malloc(11 + ft_strlen(env_line) + 3 + 1);
	if (!formatted)
		return (NULL);
	ft_strlcpy(formatted, "declare -x ", 12);
	ft_strlcat(formatted, env_line, 11 + key_len + 1);
	ft_strlcat(formatted, "=\"", 11 + key_len + 3);
	ft_strlcat(formatted, equal_sign + 1, 11 + ft_strlen(env_line) + 3);
	ft_strlcat(formatted, "\"", 11 + ft_strlen(env_line) + 4);
	return (formatted);
}

void	format_envp(char **envp)
{
	int		i;
	char	*temp;
	char	*equal_sign;
	char	*formatted;
	size_t	key_len;

	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			key_len = equal_sign - envp[i];
			formatted = get_format_line(envp[i], equal_sign, key_len);
			if (!formatted)
				return ;
			temp = envp[i];
			envp[i] = formatted;
			free(temp);
		}
		i++;
	}
}

static int	count_quotes(char *str)
{
	int	count;
	int	j;
	int	equal_found;

	count = 0;
	j = 0;
	equal_found = 0;
	while (str[j])
	{
		if (str[j] == '=' && !equal_found)
			equal_found = 1;
		else if (equal_found && str[j] == '"')
			count++;
		j++;
	}
	return (count);
}

static char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (i > 0 && str[i - 1] == '\\')
				result[j++] = str[i];
		}
		else
			result[j++] = str[i];
		i++;
	}
	free(str);
	result[j] = '\0';
	return (result);
}

char	*handle_quotes(t_ast_node *node, int *i)
{
	char	*temp;

	if (count_quotes(node->args[*i]) % 2 == 0)
		return (remove_quotes(ft_strdup(node->args[*i])));
	temp = ft_strdup(node->args[*i]);
	while (count_quotes(temp) % 2 != 0 && node->args[*i + 1])
	{
		(*i)++;
		temp = ft_strjoin_free(temp, " ");
		temp = ft_strjoin_free(temp, node->args[*i]);
	}
	temp = remove_quotes(temp);
	return (temp);
}
