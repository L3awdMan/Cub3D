/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puts_bin_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:18:20 by baelgadi          #+#    #+#             */
/*   Updated: 2025/07/30 20:21:17 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_bin_fd(const char *s, int fd)
{
	int			i;
	const char	*ptr;

	if (!s)
		return ;
	ptr = s;
	while (*ptr)
	{
		i = 7;
		while (i >= 0)
		{
			if ((*ptr >> i) & 1)
				ft_putchar_fd('1', fd);
			else
				ft_putchar_fd('0', fd);
			i--;
		}
		if (*(ptr + 1))
			ft_putchar_fd(' ', fd);
		ptr++;
	}
}
