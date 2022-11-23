/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmoghadd <pmoghadd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:50:42 by pooneh            #+#    #+#             */
/*   Updated: 2022/11/22 21:39:22 by pmoghadd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../includes/minishell.h"

/**
 * @brief when there is a $, this function is called to 
 * find out what is the variable that should expand? acc to bash behavior, every
 * charnum that comes right after that is considered a variable, except for quotes.
 * @param string clear
 * @return char* the variable part that can/may be expanded. 
 */

char	*find_variable_part(char *string)
{
	int	i;

	i = 0;
	if (string[i] && first_char_check(string[i]))
	{
		if (string[i] && (string[i] == '\'' || string[i] == '"'))
			i += skip_quotes(string + i);
		while (string[i] && ft_isalnum_ms(string[i]))
			i++;
		if (i == 0)
			return ("0");
		return (ft_substr(string, 0, i));
	}
	else
		return (ft_substr(string, 0, 1));
}

/**
 * @brief after receiving the variable, it expands the variable 
 * by goinf through the env prointer that is handed in the t_env data struct.
 * @param name 
 * @param index index of one after where the $ is seen. 
 * this is to be able to keep the string part 
 * before dollar sign. ex: abcd$USER
 * @param variable the output of find variable part function
 * @param envp environment pointer is saved in this data struct. 
 * @return int the index which should be considered for future iterartions. 
 */

int	replace_variable_value(char **name, int index, char	*variable, t_env *envp)
{
	int		i;
	int		l;
	char	*tmp_tail;
	char	*tmp_head;

	i = 0;
	l = 0;
	tmp_tail = *name + index + ft_strlen(variable);
		tmp_head = ft_substr(*name, 0, index - 1);
	while (envp)
	{
		if (!ft_strncmp(variable, envp->key, ft_strlen(envp->key)))
		{
			tmp_head = ft_strjoin_minishell(tmp_head, envp->value);
			l = ft_strlen(envp->value);
		}
		else if (!ft_strncmp(variable, "?", 1))
			tmp_head = ft_strjoin_minishell(tmp_head, envp->key);
		envp = envp->next;
	}
	if (l == 0 && (variable[0] == '"' || variable[0] == '\''))
		tmp_head = ft_strjoin_minishell(tmp_head, variable);
	tmp_head = ft_strjoin_minishell(tmp_head, tmp_tail);
	*name = tmp_head;
	return (l + index);
}

/**
 * @brief goes through the string and when founded $, expands it.
 * 
 * @param name the input string
 * @param envp environment pointer is saved in this data struct. 
 * @return char* the expanded version of input string
 */

char	*expand(char *name, t_env *envp)
{
	char	*variable;
	int		i;

	i = 0;
	while (name[i])
	{
		if (name[i] == '\'' && name[skip_quotes(name + i) - 1] == '\'')
			i += skip_quotes(name + i);
		if (name[i] == '$')
		{
			variable = find_variable_part(name + i + 1);
			i = replace_variable_value(&name, i + 1, variable, envp) - 1;
			i--;
		}
		i++;
	}
	return (name);
}
