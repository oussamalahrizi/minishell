/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 20:33:03 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/13 11:10:59 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int count_args(char **cmd_args)
{
	int len;

	len = 0;
	while (cmd_args[len])
		len++;
	return (len);
}
