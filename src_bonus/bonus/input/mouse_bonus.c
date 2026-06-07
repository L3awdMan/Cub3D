/* SECTION 4 (src_bonus/bonus/mouse_bonus.c): bonus feature #1 — mouse look.
 * MotionNotify (X event 6) accumulates the raw pointer delta in cub->mouse_dx
 * — no cursor warp, no cursor hide — so the X11 cursor stays visible and the
 * window's close button stays clickable. The per-frame loop drains mouse_dx
 * via apply_mouse_look() and rotates the view. FocusIn / FocusOut clear the
 * mouse_ready flag so the first MotionNotify after refocus only seeds the
 * baseline (no huge delta jump from cursor travel while unfocused). */

#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief MotionNotify hook. Stores baseline on the first event after focus
 * is gained (mouse_ready == 0) and otherwise accumulates the raw pointer
 * delta into cub->mouse_dx. The accumulator is drained per frame in
 * apply_mouse_look(). Out-of-window x values are ignored — they happen
 * after focus loss when the cursor lives in another window.
 * @return 0 per the MLX hook convention.
 */
int	mouse_move(int x, int y, void *param)
{
	t_cub	*cub;

	(void)y;
	cub = (t_cub *)param;
	if (x < 0 || x >= WIN_W)
		return (0);
	if (!cub->mouse_ready)
	{
		cub->mouse_ready = 1;
		cub->mouse_last_x = x;
		return (0);
	}
	cub->mouse_dx += (x - cub->mouse_last_x);
	cub->mouse_last_x = x;
	return (0);
}

/**
 * @brief FocusIn hook (X event 9). Resets mouse_ready so the next motion
 * event seeds the baseline instead of producing a giant delta jump from
 * the cursor having moved while the window was unfocused.
 * @return 0 per the MLX hook convention.
 */
int	focus_in(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	cub->mouse_ready = 0;
	cub->mouse_dx = 0;
	return (0);
}

/**
 * @brief FocusOut hook (X event 10). Same reset path — guarantees mouse_dx
 * is not accumulated against a stale last_x when focus returns.
 * @return 0 per the MLX hook convention.
 */
int	focus_out(void *param)
{
	t_cub	*cub;

	cub = (t_cub *)param;
	cub->mouse_ready = 0;
	cub->mouse_dx = 0;
	return (0);
}

/**
 * @brief Per-frame mouse-look. Drains the accumulated horizontal delta
 * into a single rotation, then zeroes the accumulator. Reuses
 * rotate_vectors() so mouse and keyboard rotation stay perfectly
 * consistent.
 */
void	apply_mouse_look(t_cub *cub)
{
	if (!cub->mouse_ready || cub->mouse_dx == 0)
		return ;
	rotate_vectors(&cub->player, (double)cub->mouse_dx * MOUSE_SENS);
	cub->mouse_dx = 0;
}

/**
 * @brief Wires up mouse-look: MotionNotify (event 6, PointerMotionMask),
 * FocusIn (event 9, FocusChangeMask) and FocusOut (event 10) hooks. The
 * cursor is intentionally left visible and not warped — the window's X
 * close button must stay clickable, and the user must be able to alt-tab
 * away without the next refocus producing a wild view spin.
 */
void	register_mouse(t_cub *cub)
{
	mlx_hook(cub->win, 6, 1L << 6, mouse_move, cub);
	mlx_hook(cub->win, 9, 1L << 21, focus_in, cub);
	mlx_hook(cub->win, 10, 1L << 21, focus_out, cub);
}
