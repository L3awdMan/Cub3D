#include "cub3d_bonus.h"
#include "mlx.h"

/**
 * @brief Reads a pixel from a texture image buffer
 * @warning Casting needed for the same reasons as put_px()
 *
 * FIX 2 (src/render/texture.c): x/y are clamped before indexing.
 * wall_x * tex->width in calc_tex_x can round to exactly tex->width on
 * corner hits (FP edge), which would cause a 1-pixel OOB heap read every
 * frame. Mirrors the tex_y clamp already in draw_wall.c:draw_tex_col.
 */
unsigned int	get_tex_pixel(t_img *tex, int x, int y)
{
	char	*px;

	if (x < 0)
		x = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y < 0)
		y = 0;
	if (y >= tex->height)
		y = tex->height - 1;
	px = tex->data + (y * tex->line_len + x * (tex->bpp / 8));
	return (*(unsigned int *)px);
}

/**
 * @brief Loads one XPM texture into tex[idx]
 */
static void	load_single_tex(t_cub *cub, int idx)
{
	t_img	*t;

	t = &cub->tex[idx];
	t->id = mlx_xpm_file_to_image(cub->mlx, cub->map.tex_path[idx],
			&t->width, &t->height);
	if (!t->id)
		exit_error(cub, "Failed to load texture");
	t->data = mlx_get_data_addr(t->id, &t->bpp, &t->line_len, &t->endian);
}

/**
 * @brief SECTION 4 (src_bonus/render/texture.c): loads one XPM file into the
 * given t_img. Used for the bonus door / floor / ceiling textures, whose
 * paths are fixed (DOOR_FACE_PATH / FLOOR_PATH / CEIL_PATH) rather than
 * read from the .cub file.
 */
static void	load_named_tex(t_cub *cub, t_img *t, char *path)
{
	t->id = mlx_xpm_file_to_image(cub->mlx, path, &t->width, &t->height);
	if (!t->id)
		exit_error(cub, "Failed to load texture");
	t->data = mlx_get_data_addr(t->id, &t->bpp, &t->line_len, &t->endian);
}

static void	load_wall_alt_textures(t_cub *cub, t_theme *th)
{
	load_named_tex(cub, &cub->wall_alt[0], th->alt[0]);
	load_named_tex(cub, &cub->wall_alt[1], th->alt[1]);
	load_named_tex(cub, &cub->wall_alt[2], th->alt[2]);
}

/**
 * @brief Loads all 4 wall textures plus the bonus door, floor and ceiling
 * textures. The door/floor/ceiling/accent paths come from the per-floor theme
 * (floor_theme), so floor 3 re-skins them while floors 1 & 2 stay unchanged.
 */
void	load_textures(t_cub *cub)
{
	int		i;
	t_theme	th;

	th = floor_theme(cub->floor);
	i = 0;
	while (i < TEX_COUNT)
	{
		load_single_tex(cub, i);
		i++;
	}
	load_named_tex(cub, &cub->door_tex, th.door);
	load_named_tex(cub, &cub->door_open_tex, th.door_open);
	load_wall_alt_textures(cub, &th);
	load_named_tex(cub, &cub->floor_tex, th.floor);
	load_named_tex(cub, &cub->ceil_tex, th.ceil);
	load_named_tex(cub, &cub->boss_ceil_tex, th.boss_ceil);
}
