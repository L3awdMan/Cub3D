/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:54:14 by baelgadi          #+#    #+#             */
/*   Updated: 2025/09/13 01:12:42 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t num_bytes)
{
	unsigned char	ch1;
	unsigned char	ch2;

	while (num_bytes-- > 0 && (*s1 || *s2))
	{
		ch1 = (unsigned char)*s1++;
		ch2 = (unsigned char)*s2++;
		if (ch1 != ch2)
			return (ch1 - ch2);
	}
	return (0);
}
