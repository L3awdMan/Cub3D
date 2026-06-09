#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Writes a pixel at (x, y) into the image buffer
 * @warning Casting is needed because otherwise *px = color would only write
 * 1 byte (the lowest of color) and everything would be only in blue
 */
void	put_px(t_img *img, int x, int y, unsigned int color)
{
	char	*px;

	if (x < 0 || y < 0 || x >= WIN_W || y >= WIN_H)
		return ;
	px = img->data + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)px = color;
}

static int	render_ui_frame(t_cub *cub)
{
	if (cub->game_state == GS_CUTSCENE)
	{
		draw_cutscene(cub);
		mlx_put_image_to_window(cub->mlx, cub->win, cub->img.id, 0, 0);
		return (1);
	}
	if (cub->game_state == GS_MISSION)
	{
		draw_mission_ui(cub);
		mlx_put_image_to_window(cub->mlx, cub->win, cub->img.id, 0, 0);
		return (1);
	}
	if (cub->game_state >= GS_INTRO)
	{
		draw_menu(cub);
		mlx_put_image_to_window(cub->mlx, cub->win, cub->img.id, 0, 0);
		return (1);
	}
	return (0);
}

/**
 * @brief Renders a complete frame: walls → sprites → minimap → crosshair,
 * then the single mlx_put_image_to_window blit. The crosshair draws last so
 * it stays on top of every world layer.
 */
static void	render_frame(t_cub *cub)
{
	if (render_ui_frame(cub))
		return ;
	cast_all_rays(cub);
	draw_sprites(cub);
	draw_projectiles(cub);
	draw_minimap(cub);
	draw_floor_hud(cub);
	draw_lives_hud(cub);
	draw_weapon(cub);
	draw_damage_flash(cub);
	draw_pickup_flash(cub);
	draw_health_bars(cub);
	draw_crosshair(cub);
	draw_endgame(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.id, 0, 0);
}

/**
 * @brief Advances one frame of gameplay simulation.
 *
 * SECTION 4: update_doors advances every door's slide progress at
 * DOOR_SPEED per millisecond so the animation runs in wall-clock time
 * regardless of frame rate. SECTION 3: update_horizon refreshes the
 * head-bob so this frame's bob matches this frame's movement state.
 */
static void	update_world(t_cub *cub)
{
	apply_movement(cub);
	apply_rotation(cub);
	apply_mouse_look(cub);
	weapon_pickup(cub);
	collect_rewards(cub);
	update_doors(cub);
	update_horizon(cub);
	update_enemies(cub);
	update_projectiles(cub);
	update_weapon(cub);
}

/**
 * @brief Main loop hook
 * @return 0 to keep MLX loop running
 *
 * SECTION 4: the world is only simulated while GS_PLAYING — on the dead/win
 * screen the scene freezes and only update_endgame (input) + render run.
 */
int	loop_hook(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	if (cub->game_state == GS_CUTSCENE)
		update_cutscene(cub);
	else if (cub->game_state == GS_PLAYING)
	{
		update_world(cub);
		update_mission_ui(cub);
	}
	else if (cub->game_state >= GS_INTRO && cub->game_state != GS_MISSION)
		update_menu(cub);
	if (cub->game_state <= GS_WIN)
		update_endgame(cub);
	render_frame(cub);
	if (!cub->floor_hud.id)
		load_floor_hud(cub);
	if (!cub->lives_hud[0].id)
		load_lives_hud(cub);
	return (0);
}
