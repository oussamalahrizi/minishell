/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exiled <exiled@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:05:34 by olahrizi          #+#    #+#             */
/*   Updated: 2023/05/06 08:06:22 by exiled           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	char	*start;
	size_t	total_len;

	if (!s1 || !s2)
		return (0);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	join = (char *) malloc(sizeof(char) * (total_len + 1));
	if (!join)
		return (NULL);
	start = join;
	ft_memcpy(join, s1, ft_strlen(s1));
	join += ft_strlen(s1);
	ft_memcpy(join, s2, ft_strlen(s2));
	join += ft_strlen(s2);
	*join = 0;
	free((void *)s1);
	return (start);
}
