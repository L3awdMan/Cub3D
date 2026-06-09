/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mission_state_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <zotaj-di@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:08:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/06/10 00:08:52 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SECTION 4 (src_bonus/bonus/mission_state_bonus.c): mission UI state flow.
 * Mission briefings are scheduled after floor entry, shown after a short
 * playable delay, then dismissed by Enter/Space back to gameplay. */

#include "cub3d_bonus.h"

void	schedule_mission_ui(t_cub *cub)
{
	cub->mission_pending = 1;
	cub->mission_due_ms = now_ms() + MISSION_DELAY_MS;
}

void	update_mission_ui(t_cub *cub)
{
	if (cub->game_state != GS_PLAYING || !cub->mission_pending)
		return ;
	if (now_ms() < cub->mission_due_ms)
		return ;
	ft_bzero(cub->keys, sizeof(cub->keys));
	cub->game_state = GS_MISSION;
}

void	draw_mission_ui(t_cub *cub)
{
	int	i;

	if (!cub->ui_mission[0].id)
		load_mission_ui(cub);
	i = cub->floor - 1;
	if (i < 0)
		i = 0;
	if (i >= MISSION_COUNT)
		i = MISSION_COUNT - 1;
	blit_full(cub, &cub->ui_mission[i], OPAQUE_KEY);
}

void	mission_input(t_cub *cub, int key)
{
	if (cub->game_state != GS_MISSION)
		return ;
	if (key != KEY_ENTER && key != KEY_SPACE)
		return ;
	cub->mission_pending = 0;
	cub->game_state = GS_PLAYING;
	ft_bzero(cub->keys, sizeof(cub->keys));
}
