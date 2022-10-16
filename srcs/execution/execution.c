/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mialbert <mialbert@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 18:48:19 by mialbert          #+#    #+#             */
/*   Updated: 2022/10/16 03:22:40 by mialbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Full_cmd contains the cmd with all its flags in seperate elements: "ls -la"
 * It is included in my data struct because it is later used in execve!
 * Cmd takes the first element and joins it with '/': "/ls"
 * Path combines the path with cmd: "usr/bin/ls" and checks its accessibility.
 */
static char	*find_path(t_data *data, size_t	group_i)
{
	size_t	i;
	char	*cmd;
	char	*path;

	i = 0;
	printf("hi\n");
	printf("group_i: %zu\n", group_i);
	cmd = ft_strjoin("/", data->group[group_i].full_cmd[0]);
	while (data->paths[i++])
	{
		path = ft_strjoin(data->paths[i - 1], cmd);
		if (access(path, F_OK | X_OK) == 0)
		{
			printf("path: %s\n", path);
			return (free(cmd), path);
		}
		else
			free(path);
	}
	free(cmd);
	return (display_error(data, "path failed", true), NULL);
}

/**
 * First I find the correct path for the next command in find_path()
 * Execve will execute another program which takes over the entire process.
 * Which is why this is done in a child process. 
 * At the very last itteration, the output is redirected to a file. 
 */
static void	child_cmd(t_data *data, size_t i, int32_t fd[2], char **env)
{
	char	*path;

	// (void)fd;
	path = find_path(data, i);
	outfiles(data, &data->group[i]);
	if (data->group->outfile == NULL)
	{
		printf("yo\n");
		dup2(fd[WRITE], STDOUT_FILENO);
	}
	close(fd[READ]);
	close(fd[WRITE]);
	ft_printf_fd(STDERR_FILENO, "data->group[i].full_cmd: %s\n", \
									*(data->group[i].full_cmd));
	printf("%s", path);
	if (execve(path, data->group[i].full_cmd, env) == -1)
	{
		free(path);
		display_error(data, "execve failed", true);
	}
}

t_builtin	builtin_check(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (&unset);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		return (&echo);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (&print_env);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (&exit_check);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (&export);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (&pwd);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (&unset);
	return (NULL);
}

/**
 * Creating new child processes for each command executed, facilitating
 * inter-process communication with pipes and redirecting output and input
 * from the commands to STDIN and STDOUT with dup2, which will then be used
 * by the next command. Closing the fds so the program doesn't wait for input.
 */
static void	exec_cmds(t_data *data, char **env)
{
	size_t		i;
	int32_t		pid;
	int32_t		fd[2];
	// t_builtin	builtin;

	i = 0;
	while (i < (size_t)data->groupc)
	{
		// infiles(data, &data->group[i]);
		// pipe(fd); <----- stopped working bc of this wtf 
		// builtin = builtin_check(data->group[i].full_cmd[0]);
		pid = fork();
		if (pid == -1)
			display_error(data, "fork failed", true);
		if (pid == 0)
			child_cmd(data, i, fd, env);
		waitpid(pid, NULL, 0);
		dup2(fd[READ], STDIN_FILENO);
		close(fd[READ]);
		close(fd[WRITE]);
		i++;
	}
}

void	execution(t_data *data, char **env)
{
	exec_cmds(data, env);
	free_at_exit(data);
}
