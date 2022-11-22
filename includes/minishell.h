/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mialbert <mialbert@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 18:45:27 by pmoghadd          #+#    #+#             */
/*   Updated: 2022/11/22 22:23:48 by mialbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/signal.h>
# include <sys/wait.h>
# include <signal.h>
# include <unistd.h>
# include </Users/mialbert/goinfre/.brew/opt/readline/include/readline/readline.h>
# include </Users/mialbert/goinfre/.brew/opt/readline/include/readline/history.h>
# include "../libs/libft/includes/libft.h"

# define READ 0
# define WRITE 1
# define PROMPT "🦇MiShell: "

int16_t	exit_code;
// #define DEBUG 1

typedef struct s_group	t_group;
typedef struct s_data	t_data;
typedef bool			(*t_builtin)(t_data *, t_group *);
char	debugBuf[6969];

void	this_is_debug_yo(void);

/**
 *  EMPTY	- ""
	CMD		- command not found
	TOKEN	- syntax error near unexpexted token
	ISDIR	- is a directory
	NODIR	- No such file or directory				1
	IDENT	- not a valid identifier				1
	NOEVENT	- event not found
	INVOPT	- invalid option
	ARGS	- too many arguments					1
	HOME	- HOME not set
	PERM	- permission denied
	NUMARG	- numeric arugment required
	ERR_COUNT - undefined error
*/
enum e_errno
{
	EMPTY,
	CMD,
	TOKEN,
	ISDIR,
	NODIR,
	IDENT,
	NOEVENT,
	INVOPT,
	ARGS,
	HOME,
	PERM,
	NUMARG,
	ERR_COUNT
};

typedef struct s_outfile
{
	char	*name;
	bool	append;
	struct s_outfile	*next;
}	t_outfile;

typedef struct s_infile
{
	char	*name;
	bool	here_doc;
	struct s_infile	*next;
}	t_infile;

/**
 * This encompasses everything between/before/after pipelines. 
 * Information that purtains to one command. 
 * -> <infile1 <infile2 cmd >outfile1 >outfile2 <- | cmd2 >outfile3
 * We have an array of these structs in the data struct. To seperate
 * information for each "pipegroup".
 */

typedef struct s_group
{
	char		**full_cmd;
	t_infile	*infile;
	t_outfile	*outfile;
	t_builtin	builtin;
	int32_t		read_in;
	int32_t		read_out;
	int32_t		commandc;
}	t_group;

typedef struct s_env
{
	char			*keyvalue;
	char			*key;
	char			*value;
	bool			printed;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	t_env		*envp_head;
	t_group		*group;
	size_t		groupc;
	int32_t		tmp_fd;
}	t_data;

typedef struct t_parsing
{
	char				*infile;
	char				*outfile;
	char				*command;
	struct t_parsing	*next;
}	t_parsing;

typedef struct token
{
	char			*command;
	char			**command_ops;
	t_infile		*infile;
	t_outfile		*outfile;
	size_t			counter_infile;
	size_t			counter_outfile;
	int				read_in;
	int				read_out;
	struct token	*next;
}	t_token;

/*		remove later	*/
void	free_fds();

/*		initialize		*/

void	lstaddback(t_infile **lst, t_infile *new);
void	lstaddback_out(t_outfile **lst, t_outfile *new);
void	initialize(t_group	**data);
void	words_init(t_group	**info, char *name, t_env *envp);
void	out_file_init(t_group	**info, char *s, char *name, t_env *envp);
void	in_file_init(t_group	**info, char *s, char *name, t_env *envp);

/*		lexical_scan	*/
int		skip_quotes(char *s);
int		special_chars(t_group **info, char *s, t_env *envp);
int		skip_chars(char *s);
int		quoted_word_extract(t_group **info, char *s, t_env *envp);
int		normal_word_extract(t_group **info, char *s, t_env *envp);
char * rm_quotes_(char *name);

void	first_initialization(char **pipe_wise_splitted_array, t_data *data);
char	*expand(char *name, t_env *envp);
char	**ft_split_shell(const char *s, char c);
int		skip_spaces(char *s);
void	lstaddback_out(t_outfile **lst, t_outfile *new);
void	lstaddback(t_infile **lst, t_infile *new);
int		check_neighbouring_chars(char *s);
int		check_input_before_handling(char *s);
char	**ft_split_shell(const char *s, char c);
bool	ft_isalnum_ms(int32_t c);
int		first_char_check(char c);
void	make_token(char *s, t_group **info, t_env *envp);
char	*remove_quotes(char *name, int start);
char	*ft_strjoin_minishell(char *str1, char *str2);
int		err_parser(char *msg, char c);

/* general */
void	display_error(int8_t nbr, char *str[], t_data *data, t_group *group);
void	free_at_exit(t_data *data);
void	free_data(t_data *data);
void	parser(char *str, t_env *envp, t_data *data);
void	free_groups(t_data *data);
char	**join_err(char *str1, char *str2);
char	*join_builtin(char *str1, char *str2);

/* environment variable linked list handlers */
char	**env_2darr(t_data *data, t_env *lst);
void	print_group(void);
t_env	*find_node(t_env *lst, char *key);
void	lst_addback(t_data *data, t_env *new);
void	free_env_node(t_env *lst, bool free_all);
char	**env_split(char *str, char del);
void	print_env(t_env *lst);

/* execution */
bool	check_key(char *key);
bool	pwd(t_data *data, t_group *group);
void	env_innit(t_data *data, char **envp);
void	execution(t_data *data);
bool	infiles(t_data *data, t_group *group);
bool	outfiles(t_data *data, t_group *group);
char	*get_path(t_data *data);
char	*find_path(t_data *data, char *cmd_name);
void	path_innit(t_data *data);
char	*find_new_path(char *str, char *path);
char	*absolute_or_relative(char *path, char *old_path);
char	*relative_path(char *relative, char *pwd);
void	set_exitcode(void);
void	shlvl_msg(t_env *envp_head, bool exit);
void	greeting_msg(t_env *envp_head);

/* builtins: */
bool	exit_check(t_data *data, t_group *group);
bool	cd(t_data *data, t_group *group);
bool	echo(t_data *data, t_group *group);
bool	exit_check(t_data *data, t_group *group);
bool	export(t_data *data, t_group *group);
bool	export_add(t_data *data, t_group *group);
bool	init_pwd_size(t_data *data);
bool	unset(t_data *data, t_group *group);
bool	env(t_data *data, t_group *group);

#endif
