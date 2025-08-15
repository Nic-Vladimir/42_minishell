/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:31:05 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 15:23:01 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_expand_value(t_env *env, const char *input, int start, int end)
{
	char	*var_name;
	char	*value;
	int		len;

	len = end - start;
	if (len == 1 && input[start] == '?')
		return (ft_itoa(env->last_exit_code));
	if (len == 1 && input[start] == '$')
		return (ft_itoa(env->shell_pid));
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_memset(var_name, 0, len + 1);
	ft_strlcpy(var_name, &input[start], len + 1);
	value = get_env_value(env, var_name);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

static int	parse_variable_name(const char *input, int *i)
{
	int	start;

	(*i)++;
	start = *i;
	if (input[*i] == '$' || input[*i] == '?')
		(*i)++;
	else
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
	return (start);
}

static char	*append_single_char(char *expanded, char c)
{
	char	*temp;

	temp = malloc(2);
	if (!temp)
	{
		free(expanded);
		return (NULL);
	}
	temp[0] = c;
	temp[1] = '\0';
	expanded = ft_strjoin_free(expanded, temp);
	free(temp);
	return (expanded);
}

char	*expand_var(t_env *env, const char *input)
{
	char	*expanded;
	char	*temp;
	int		i;
	int		start;

	expanded = ft_strdup("");
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1])
		{
			start = parse_variable_name(input, &i);
			temp = get_expand_value(env, input, start, i);
			expanded = ft_strjoin_free(expanded, temp);
			free(temp);
		}
		else
		{
			expanded = append_single_char(expanded, input[i]);
			if (!expanded)
				return (NULL);
			i++;
		}
	}
	return (expanded);
}
