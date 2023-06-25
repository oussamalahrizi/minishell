/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 04:14:34 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 22:08:51 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  UTILS_H
# define UTILS_H

# include "../../minishell.h"

void	sortby_ascii(char **strings, int numstrings);
int		get_flag(char *cmd);
int		check_for_sign(char *cmd);
int		check_syntax_export(char *cmd);
char	**get_env_tab(t_env *env, int *i);
void	print_util(t_env *env, char *array, int fd);
void	print_err_syntax(char *name);
char	*trim_for_join(char const *s1, char const *set);
void	joining(char *name, t_env *env, char *value, int flag);
void	print_export(t_env *env, int fd);
void	pwd_supp(t_env *env);
void	oldpwd_supp(t_env *env);
void	pwd_failed(t_env *env, char *string);
void	change_pwd(t_env *env);
void	change_oldpwd(t_env *env);
void	change_pwd_failed(t_env *env, char *string);
int		cd_supp(char *temp, t_env *env, int flag);
void	error_cmd(char *str, int code);
void	cd_error(char *cmd);
void	cd_error_two(char *temp, t_env *env);
int		if_cd_alone(char **cmd_args, t_env *env);
void	print_cases(char *name, char *value, int fd, int flag);

#endif
