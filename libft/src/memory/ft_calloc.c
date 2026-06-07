/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:54:14 by baelgadi          #+#    #+#             */
/*   Updated: 2025/07/29 20:57:03 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nelem, size_t elsize)
{
	void	*memory;
	size_t	total_bytes;

	if (nelem && elsize)
	{
		total_bytes = nelem * elsize;
		if (total_bytes / elsize != nelem)
			return (NULL);
	}
	else
		total_bytes = 0;
	memory = malloc(total_bytes);
	if (!memory)
		return (NULL);
	ft_bzero(memory, total_bytes);
	return (memory);
}
