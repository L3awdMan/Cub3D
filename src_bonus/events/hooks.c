#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Handles KeyPress: mark key as held in the keys[] state array
 * @return 0 as per MLX hook convention
 *
 * SECTION 4 (src_bonus/events/hooks.c): KEY_M toggles the minimap and KEY_E
 * toggles the faced door. The edge check (key not already held) makes one
 * tap = one action even though X11 autorepeat re-fires KeyPress on hold.
 */
int	key_press(int key, void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	if (key >= 0 && key < 65536 && !cub->keys[key])
	{
		if (cub->game_state >= GS_INTRO && cub->game_state != GS_FADEOUT)
			menu_input(cub, key);
		if (key == KEY_M)
			cub->show_minimap = !cub->show_minimap;
		if (key == KEY_E)
			toggle_door(cub);
	}
	if (key >= 0 && key < 65536)
		cub->keys[key] = 1;
	if (key == KEY_ESC)
		close_hook(param);
	return (0);
}

/**
 * @brief Handles KeyRelease: mark key as NOT held in the keys[] state array
 * @return 0 as per MLX hook convention
 */
int	key_release(int key, void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	if (key >= 0 && key < 65536)
		cub->keys[key] = 0;
	return (0);
}

/**
 * @brief Handles Window close (ESC or X button)
 * @return 0 (unreachable after exit, just keeps hook signature)
 */
int	close_hook(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	mlx_loop_end(cub->mlx);
	cub_destroy(cub);
	exit(0);
	return (0);
}

/**
 * @brief Registers all event hooks with MLX
 * @note 2 = KeyPress, 3 = KeyRelease, 17 = DestroyNotify
 *
 * SECTION 4 (src_bonus/events/hooks.c): also wires the bonus mouse-look hook
 * via register_mouse() (event 6, MotionNotify) and the left-click weapon-fire
 * hook (event 4, ButtonPress).
 */
void	register_hooks(t_cub *cub)
{
	mlx_hook(cub->win, 2, 1L << 0, key_press, cub);
	mlx_hook(cub->win, 3, 1L << 1, key_release, cub);
	mlx_hook(cub->win, 17, 0, close_hook, cub);
	mlx_hook(cub->win, 4, 1L << 2, weapon_fire, cub);
	register_mouse(cub);
	mlx_loop_hook(cub->mlx, loop_hook, cub);
}
