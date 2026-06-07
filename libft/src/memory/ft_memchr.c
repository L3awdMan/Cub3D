/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:54:14 by baelgadi          #+#    #+#             */
/*   Updated: 2025/07/29 21:19:42 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *memory, int c, size_t num_bytes)
{
	const unsigned char	*memory_ptr;
	unsigned char		target;

	memory_ptr = memory;
	target = c;
	while (num_bytes--)
	{
		if (*memory_ptr == target)
			return ((void *)memory_ptr);
		memory_ptr++;
	}
	return (NULL);
}
