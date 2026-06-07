/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:21:21 by baelgadi          #+#    #+#             */
/*   Updated: 2025/08/30 01:15:26 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	handle_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

static double	handle_whole_part(const char **str)
{
	double	whole_part;

	whole_part = 0.0;
	while (**str >= '0' && **str <= '9')
	{
		whole_part = whole_part * 10.0 + (double)(**str - '0');
		(*str)++;
	}
	return (whole_part);
}

static double	handle_fraction_part(const char **str)
{
	double	fraction;
	double	scale;

	fraction = 0.0;
	scale = 1.0;
	if (**str == '.')
	{
		(*str)++;
		while (**str >= '0' && **str <= '9')
		{
			fraction = fraction * 10.0 + (double)(**str - '0');
			scale = scale * 10.0;
			(*str)++;
		}
	}
	return (fraction / scale);
}

double	ft_atof(const char *str)
{
	int		sign;
	double	before_dot;
	double	after_dot;

	sign = handle_sign(&str);
	before_dot = handle_whole_part(&str);
	after_dot = handle_fraction_part(&str);
	return ((before_dot + after_dot) * sign);
}
