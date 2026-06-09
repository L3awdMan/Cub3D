/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutscene_path_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:06:41 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:06:41 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/* SECTION 4 (cutscene_path_bonus.c): builds the CUT_DIR-relative XPM path for
 * one cutscene frame. cutscene_prefix() maps the cutscene id to its frame-name
 * stem and pad3() zero-pads the frame index to three digits. */

static char	*cutscene_prefix(int id)
{
	if (id == CUT_END_F2)
		return ("end_floor2/end_floor2_");
	if (id == CUT_END_F3)
		return ("end_floor3/end_floor3_");
	return ("begin_floor1/begin_floor1_");
}

static char	*pad3(int n)
{
	char	*s;

	s = ft_calloc(4, sizeof(char));
	if (!s)
		return (NULL);
	s[0] = '0' + (n / 100) % 10;
	s[1] = '0' + (n / 10) % 10;
	s[2] = '0' + n % 10;
	return (s);
}

char	*cutscene_path(int id, int frame)
{
	char	*num;
	char	*mid;
	char	*file;
	char	*path;

	num = pad3(frame);
	if (!num)
		return (NULL);
	mid = ft_strjoin(cutscene_prefix(id), num);
	free(num);
	if (!mid)
		return (NULL);
	file = ft_strjoin(mid, ".xpm");
	free(mid);
	if (!file)
		return (NULL);
	path = ft_strjoin(CUT_DIR, file);
	free(file);
	return (path);
}
