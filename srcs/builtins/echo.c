/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mialbert <mialbert@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 03:04:01 by mialbert          #+#    #+#             */
/*   Updated: 2022/10/08 04:47:44 by mialbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	echo(t_data *data, t_group *group)
{
	bool	option;

	(void)data;
	option = false;
	if (ft_strncmp("-n", (group->full_cmd[1]), 2))
		option = true;
	while (*(group->full_cmd))
		printf("%s", *group->full_cmd++);
	if (option)
		printf("\n");
}
