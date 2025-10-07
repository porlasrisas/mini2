/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:50:40 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/07 17:35:16 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define TOKENS_LEN 256
# define PATH_MAX 4096

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

/* Global variable: only signal allowed by subject */
extern int				g_signal;

/* Data structures */

typedef struct s_hdoc
{
	char				*delim;
	int					no_expand;
	struct s_hdoc		*next;
}						t_hdoc;

typedef struct s_redir
{
	int					in_fd;
	int					out_fd;
	int					append_fd;
	int					heredoc_fd;
	int					has_redir_in;
	int					heredoc_no_expand;
	char				*heredoc_delim;
	t_hdoc				*heredocs;
	char				*created_rollback_path;
}						t_redir;

typedef struct s_cmd
{
	char				**argv;
	t_redir				redir;
	int					is_builtin;
	int					skip_execution;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_parser
{
	char				*input;
	t_cmd				*cmds;
	int					pipe_count;
}						t_parser;

typedef struct s_shell
{
	char				**envp;
	t_parser			parser;
	int					in_heredoc;
	int					last_status;
}						t_shell;

typedef struct s_loop_ctx
{
	char				*input;
	int					save_in;
	int					save_out;
}						t_loop_ctx;

typedef struct s_exec_ctx
{
	int					n;
	int					(*pfd)[2];
	pid_t				*pids;
	int					i;
	int					j;
	int					k;
	int					status;
	int					last_status;
	int					ret;
	struct sigaction	ign;
	struct sigaction	old_int;
	t_cmd				*cur;
	t_cmd				*node;
	t_cmd				**cmd_arr;
}						t_exec_ctx;

/* Parser context used across parser modules */
typedef struct s_parse_ctx
{
	t_cmd				**current_cmd;
	char				*input;
	int					*i;
	t_cmd				**cmds;
	int					last_status;
}						t_parse_ctx;

/* Parser expand context */
typedef struct s_expand_ctx
{
	char				**envp;
	const char			*status_str;
	size_t				status_len;
	char				*dst;
	int					j;
}						t_expand_ctx;

/* Signal handling */
void					sig_handler(int signo);
void					setup_signals(void);

/* Core shell functions */
int						shell_loop(char **envp);

t_cmd					*parse_input(char *input, char **envp, int last_status);
t_cmd					*parse_build(char *input, char **envp, int last_status);
int						execute_cmds(t_cmd *cmds, t_shell *shell);
void					free_cmds(t_cmd *cmds);

/* Environment helpers */
char					**env_dup(char **envp);
char					*get_env_var(char *var, char **envp);
int						env_set(char ***envp, const char *name,
							const char *value);
int						env_unset(char ***envp, const char *name);

/* env utils */
char					*env_make_entry(const char *name, const char *value);
int						env_init_with_entry(char ***envp, char *entry);
int						env_replace_at(char **envp, int idx, char *entry);
int						env_append_entry(char ***envp, char *entry);
int						env_remove_at(char ***envp, int idx);
int						env_index_of(char **envp, const char *name);

/* Builtin commands */
int						builtin_echo(char **argv);
int						builtin_cd(char **argv, t_shell *shell);
int						builtin_pwd(void);
int						builtin_export(char **argv, t_shell *shell);
int						builtin_unset(char **argv, t_shell *shell);
int						builtin_env(char **envp);
int						builtin_exit(char **argv, int last_status);
void					print_export_sorted(char **envp);

/* Shell loop helpers */
int						run_single_builtin(t_shell *shell, t_cmd *cmd,
							int *save_in, int *save_out);
char					*read_input(t_shell *shell);
int						handle_input_result(t_shell *shell, t_loop_ctx *c);
void					process_line(t_shell *shell, t_loop_ctx *c);
int				is_blank_str(const char *s);
int				should_skip_single(t_cmd *cmd);
void			exec_after_parsing(t_shell *shell, t_loop_ctx *c);
void					init_shell_state(t_shell *shell,
							struct sigaction *sa_pipe, t_loop_ctx *c,
							char **envp);

/* t_parse_ctx already defined */

/* Parser utilities */
char					*parse_token(const char *input, int *i);
char					*process_token(char *token, char **envp, int expand,
							int last_status);
char					*expand_env_var(char *token, char **envp,
							int last_status);
char					*remove_quotes_preserving_expand(char *token,
							int *expand_out);

t_cmd					*add_cmd(t_cmd *cmds, char **argv);
int						count_cmd_list(t_cmd *cmds);
int						append_arg_to_cmd(t_cmd *cmd, char *arg);
int						token_should_expand(const char *token);
void					mark_non_pipeline_skips(t_cmd *cmds);
void					append_or_start_cmd_ctx(t_parse_ctx *c, int *argc,
							char ***argv, char *processed);

/* Parser main loop helpers (parser_build_loop.c) */
int						parse_main_loop(t_parse_ctx *c, int *argc, char ***argv,
							char **envp);

/* Parser loop external helpers */
int						skip_spaces(t_parse_ctx *c);
void					cleanup_parse_failure(char ***argv, t_cmd **cmds);
int						process_token_and_append(t_parse_ctx *c, int *argc,
							char ***argv, char **envp);
int						handle_pipe_or_redir(t_parse_ctx *c, int *argc,
							char ***argv, int *parse_error);
int						loop_iteration(t_parse_ctx *c, int *argc, char ***argv,
							char **envp);
int						pipe_precheck(t_parse_ctx *c, int *argc, char ***argv);
int						pipe_postcheck(t_parse_ctx *c, char ***argv);

void					parse_redir(t_parse_ctx *c, int *argc, char ***argv,
							int *parse_error);

/* Parser redir helpers (utils / io / parse) */
void					attach_new_cmd_if_needed(t_parse_ctx *c, int *argc,
							char ***argv);
int						detect_quote_types(const char *s, int *had_single,
							int *had_double);
char					classify_redir_and_advance(t_parse_ctx *c, int *append,
							int *is_heredoc);
void					redir_output(t_cmd *cmd, const char *filename,
							int append);
void					redir_input_or_heredoc(t_cmd *cmd, const char *filename,
							int is_heredoc, int no_expand_heredoc);
char					*get_filename_noq(t_parse_ctx *c, int is_heredoc,
							int *no_expand_heredoc, int *parse_error);

/* Redirection handling */
void					handle_heredoc(t_cmd *cmd, t_shell *shell);
int						apply_redirections(t_cmd *cmd);
void					restore_redirections(int save_in, int save_out);
void					preprocess_heredocs(t_cmd *cmds, t_shell *shell);
int						apply_redirs_parent(t_cmd *cmd, int *save_stdin,
							int *save_stdout);
void					restore_redirs_parent(int save_stdin, int save_stdout,
							t_cmd *cmd);
void					exec_command_child(t_cmd *cmd, t_shell *shell,
							int in_pipeline);
void					exec_search_in_path(t_cmd *cmd, t_shell *shell);
void					exec_path_or_error(t_cmd *cmd, t_shell *shell);
int						hd_process_list(t_cmd *cmd, t_shell *shell,
							int *assigned_fd);
int						read_heredoc_into_pipe(int wfd, const char *delim,
							int no_expand, t_shell *shell);
void						hd_pick_source(t_hdoc *node, t_cmd *cmd, const char **delim,
							int info[2]);

/* Executor core orchestration */
int					init_ctx(t_exec_ctx *s, t_cmd *cmds);
void				fork_children(t_exec_ctx *s, t_shell *shell);

/* Error helpers */
void					ms_perror(const char *subject);
void					ms_error2(const char *subject, const char *msg);
void					ms_syntax_error(const char *tok);

/* Terminal mode helpers */
void					configure_terminal(void);
void					restore_terminal(void);

/* IO helpers */
char					*ms_read_line_fd(int fd);

/* noop: mantenido por compatibilidad de includes */

#endif