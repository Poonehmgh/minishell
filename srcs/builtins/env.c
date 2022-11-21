/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mialbert <mialbert@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 01:52:49 by mialbert          #+#    #+#             */
/*   Updated: 2022/11/21 03:23:33 by mialbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env(t_env *lst)
{
	while (lst != NULL)
	{
		if (lst->value && lst->keyvalue && *lst->value)
			ft_printf_fd(STDOUT_FILENO, "%s=%s\n", lst->key, lst->value);
		lst = lst->next;
	}
}

/**
 * First I check for valid input. "env env env" should work, 
 * any other input should throw an error. 
 * Printing out the environmental variables that have a value, 
 * otherwise they are ignored. 
 */
bool	env(t_data *data, t_group *group)
{
	size_t	i;

	i = 0;
	if (!group->full_cmd || !group->full_cmd[i])
		return (false);
	while (group->full_cmd[i] && ft_strncmp(group->full_cmd[i], \
							"env", ft_strlen(group->full_cmd[i])) == 0)
		i++;
	if (group->full_cmd[i] != NULL)
	{
		ft_printf_fd(STDERR_FILENO, "env: %s: No such file or directory", \
														group->full_cmd[i]);
		return (true);
	}
	return (print_env(data->envp_head), true);
}
