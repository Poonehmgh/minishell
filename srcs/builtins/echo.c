/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mialbert <mialbert@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 03:04:01 by mialbert          #+#    #+#             */
/*   Updated: 2022/11/19 19:43:51 by mialbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_echo(bool	newline, char **str)
{
	size_t	i;

	i = 0;
	while ((str[i]))
	{
		if (i != 0)
			printf(" ");
		printf("%s", str[i]);
		i++;
	}
	if (newline)
		printf("\n");
}

/**
 * @brief Echo prints out whatever comes afterwards with a newline 
 * char afterwards. -n cancels out this newline character.
 * This is only registered with one '-' and any amount of 'n'
 * anything in between cancels it out
 * Valid newline cancel: echo -n -n -nnnnnn -nn Hello
 * Invalid: echo -nnnn-n Hello
 */
void	echo(t_data *data, t_group *group)
{
	bool	newline;
	char	**echo;
	size_t	i;

	i = 1;
	(void)data;
	newline = true;
	echo = group->full_cmd;
	if (!echo || !echo[1]) // !echo check is there to avoid making the other check segfault
	{
		echo[0] = ft_strdup("");
		return (print_echo(newline, echo));
	}	
	echo++;
	
	while ((*echo && ft_strncmp("-n", (*echo), 2) == 0))
	{
		while ((*echo)[i])
		{
			if ((*echo)[i++] != 'n')
				return (print_echo(newline, echo));
		}
		echo++;
		newline = !true;
		i = 1;
	}
	return (print_echo(newline, echo));
}
