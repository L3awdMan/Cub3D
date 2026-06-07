/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:54:14 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/20 07:00:51 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *mem1, const void *mem2, size_t num_bytes)
{
	const unsigned char	*ptr_mem1;
	const unsigned char	*ptr_mem2;

	ptr_mem1 = mem1;
	ptr_mem2 = mem2;
	while (num_bytes--)
	{
		if (*ptr_mem1 != *ptr_mem2)
			return (*ptr_mem1 - *ptr_mem2);
		ptr_mem1++;
		ptr_mem2++;
	}
	return (0);
}
