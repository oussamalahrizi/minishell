/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:25:38 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/12 05:12:56 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;

	if (!s1)
		return (0);
	if (!set)
		return ((char *)s1);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	i = ft_strlen(s1) - 1;
	while (s1[i] && ft_strrchr(set, s1[i]))
		i--;
	return (ft_substr(s1, 0, i + 1));
}

// #include <stdio.h>
// int main()
// {
// 	printf("%s", ft_strtrim("",""));
// 	return (0);
// }