/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 08:47:39 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/19 14:01:11 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

char **append_string(char **array, char *str)
{
     char **new;

     int i = 0;
     while (array[i])
          i++;
     new = malloc(sizeof(char *) * (i + 2));
     i = 0;
     while (array[i])
     {
          new[i] = ft_strdup(array[i]);
          i++;
     }
     new[i++] = ft_strdup(str);
     new[i] = 0;
     return (new);
}

char **here_doc(char *del)
{
     int i;
     char *str;
     char **array;
     char **temp;

     i = 0;
     array = malloc(sizeof(char *));
     *array = 0;
     while(1)
     {
          str = readline("> ");
          if (!str)
               return (array);
          if (!ft_strcmp(del, str))
          {
               free(str);
               break ;
          }
          str = ft_strjoin(str, "\n");
          temp = array;
          array = append_string(array, str);
          free_double_char(temp);
          i++;
          free(str);
     }
     return (array);
}