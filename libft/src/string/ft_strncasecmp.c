/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncasecmp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 03:42:10 by baelgadi          #+#    #+#             */
/*   Updated: 2025/09/27 03:11:43 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncasecmp(const char *s1, const char *s2, size_t num_bytes)
{
	unsigned char	ch1;
	unsigned char	ch2;

	while (num_bytes-- > 0 && (*s1 || *s2))
	{
		ch1 = (unsigned char)ft_tolower((unsigned char)*s1++);
		ch2 = (unsigned char)ft_tolower((unsigned char)*s2++);
		if (ch1 != ch2)
			return (ch1 - ch2);
	}
	return (0);
}
