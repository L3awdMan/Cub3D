# ─────────────────────────────────
#             COLORS
# ─────────────────────────────────

RESET     = \033[0m
RED       = \033[31m
GREEN     = \033[32m
YELLOW    = \033[33m
BLUE      = \033[34m
MAGENTA   = \033[35m
CYAN      = \033[36m

# ─────────────────────────────────
#         CONFIGURATION
# ─────────────────────────────────

NAME      = cub3D
# SECTION 4 (Makefile): separate bonus binary. Subject mandates mandatory and
# bonus are evaluated separately, so bonus sources live in their own src_bonus/
# tree and link into a distinct cub3D_bonus executable.
BONUS_NAME = cub3D_bonus
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -Wno-incompatible-pointer-types -MMD -MP

INCLUDES  = -I./include -I./libft/include -I./mlx

LIBFT_DIR = ./libft
LIBFT     = $(LIBFT_DIR)/libft.a

MLX_DIR   = ./mlx
MLX_LIB   = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

OBJ_DIR        = obj
BONUS_OBJ_DIR  = obj_bonus

# ─────────────────────────────────
#          SOURCE FILES
# ─────────────────────────────────

PARSING =	parse_file		\
			parse_elements	\
			parse_colors	\
			parse_map		\
			validate_map	\
			init_player

EXECUTION =	raycaster	\
			ray_utils

RENDER =	render		\
			draw_wall	\
			draw_bg		\
			texture		\
			shade

EVENTS =	hooks		\
			movement	\
			rotation

MAIN =	main	\
		cleanup

SRCS = $(addprefix src/, $(addsuffix .c, $(MAIN))) \
	$(addprefix src/parsing/, $(addsuffix .c, $(PARSING))) \
	$(addprefix src/execution/, $(addsuffix .c, $(EXECUTION))) \
	$(addprefix src/render/, $(addsuffix .c, $(RENDER))) \
	$(addprefix src/events/, $(addsuffix .c, $(EVENTS)))

OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# ─────────────────────────────────
#       BONUS SOURCE FILES
# ─────────────────────────────────
# SECTION 4 (Makefile): bonus tree mirrors the mandatory layout under
# src_bonus/. B_* lists are kept independent of the mandatory lists so bonus
# files (minimap, mouse, doors, sprites...) can be added without ever leaking
# into the mandatory build. B_BONUS holds bonus-only modules in src_bonus/bonus/.

B_PARSING =		parse_file		\
				parse_elements	\
				parse_colors	\
				parse_map		\
				validate_map	\
				init_player

B_EXECUTION =	raycaster	\
				ray_utils

B_RENDER =		render		\
				draw_wall	\
				draw_bg		\
				texture		\
				shade

B_EVENTS =		hooks		\
				movement	\
				rotation

B_MAIN =		main	\
				cleanup

B_BONUS =		input/mouse_bonus			\
				minimap/minimap_bonus		\
				minimap/minimap_draw_bonus	\
				doors/doors_bonus			\
				doors/door_use_bonus		\
				doors/door_draw_bonus		\
				entities/sprites_bonus		\
				entities/anim_sprite_bonus	\
				entities/floor_anims_bonus	\
				entities/sprite_col_bonus	\
				entities/sprite_draw_bonus	\
				entities/enemy_bonus		\
				entities/enemy_ai_bonus		\
				entities/enemy_move_bonus	\
				weapons/weapon_bonus		\
				weapons/weapon_draw_bonus	\
				weapons/weapon_blit_bonus	\
				effects/damage_flash_bonus	\
				effects/heal_flash_bonus		\
				effects/reward_popup_bonus	\
				hud/floor_hud_bonus		\
				hud/lives_hud_bonus		\
				hud/lives_hud_draw_bonus	\
				hud/hud_bar_bonus		\
				hud/hud_bar_draw_bonus		\
				cutscenes/cutscene_path_bonus	\
				cutscenes/cutscene_bonus	\
				cutscenes/cutscene_draw_bonus	\
				ui/theme_bonus			\
				ui/menu_bonus			\
				ui/menu_frame_bonus		\
				ui/menu_input_bonus		\
				ui/difficulty_bonus		\
				ui/mission_bonus		\
				ui/mission_state_bonus		\
				ui/endgame_bonus		\
				ui/endgame_state_bonus		\
				ui/endgame_input_bonus		\
				progression/floor_switch_bonus	\
				progression/restart_bonus	\
				projectiles/projectile_bonus	\
				projectiles/projectile_draw_bonus	\
				system/texture_free_bonus

BONUS_SRCS = $(addprefix src_bonus/, $(addsuffix .c, $(B_MAIN))) \
	$(addprefix src_bonus/parsing/, $(addsuffix .c, $(B_PARSING))) \
	$(addprefix src_bonus/execution/, $(addsuffix .c, $(B_EXECUTION))) \
	$(addprefix src_bonus/render/, $(addsuffix .c, $(B_RENDER))) \
	$(addprefix src_bonus/events/, $(addsuffix .c, $(B_EVENTS)))

ifneq ($(strip $(B_BONUS)),)
BONUS_SRCS += $(addprefix src_bonus/bonus/, $(addsuffix .c, $(B_BONUS)))
endif

BONUS_OBJS = $(patsubst src_bonus/%.c, $(BONUS_OBJ_DIR)/%.o, $(BONUS_SRCS))
BONUS_DEPS = $(BONUS_OBJS:.o=.d)

# ─────────────────────────────────
#        COMPILATION RULE
# ─────────────────────────────────

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(@D)
	@printf "Compiling $(BLUE)%-45s$(RESET)" $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ && printf "✅\n" || printf "❌\n"

$(BONUS_OBJ_DIR)/%.o: src_bonus/%.c
	@mkdir -p $(@D)
	@printf "Compiling $(BLUE)%-45s$(RESET)" $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ && printf "✅\n" || printf "❌\n"

# ─────────────────────────────────
#            TARGETS
# ─────────────────────────────────

all: $(LIBFT) $(MLX_LIB) $(NAME)

$(LIBFT):
	@printf "$(MAGENTA)Building libft...$(RESET)\n"
	@make -C $(LIBFT_DIR)

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		printf "$(RED)MiniLibX not found at $(MLX_DIR)$(RESET)\n"; \
		exit 1; \
	fi
	@printf "$(MAGENTA)Building MinilibX...$(RESET)\n"
	@$(MAKE) -s -C $(MLX_DIR)

$(NAME): $(OBJS)
	@printf "$(MAGENTA)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(MLX_FLAGS) -o $(NAME)
	@printf "$(GREEN)✅ $(NAME) ready$(RESET)\n"

# SECTION 4 (Makefile): real bonus rule. Builds cub3D_bonus from the src_bonus/
# tree, sharing libft and MinilibX with the mandatory build. Objects and deps
# land in $(BONUS_OBJ_DIR)/ so they never collide with mandatory objects.
bonus: $(LIBFT) $(MLX_LIB) $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	@printf "$(MAGENTA)Linking $(BONUS_NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) $(BONUS_OBJS) -L$(LIBFT_DIR) -lft $(MLX_FLAGS) \
		-o $(BONUS_NAME)
	@printf "$(GREEN)✅ $(BONUS_NAME) ready$(RESET)\n"

# ─────────────────────────────────
#            CLEANING
# ─────────────────────────────────

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR) $(BONUS_OBJ_DIR)
	@printf "$(YELLOW)🧹 Objects and dependencies deleted (clean)$(RESET)\n"

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(BONUS_NAME)
	@printf "$(RED)❌ $(NAME) removed (fclean)$(RESET)\n"

re: fclean all

# ─────────────────────────────────
#         HELP & FOOTER
# ─────────────────────────────────

help:
	@printf "$(BLUE)Available targets:$(RESET)\n"
	@printf "$(YELLOW)all      $(RESET)– Build $(NAME)\n"
	@printf "$(YELLOW)bonus    $(RESET)– Build $(BONUS_NAME)\n"
	@printf "$(YELLOW)clean    $(RESET)– Delete .o and .d files\n"
	@printf "$(YELLOW)fclean   $(RESET)– Full clean (including binaries)\n"
	@printf "$(YELLOW)re       $(RESET)– fclean + all\n"

-include $(DEPS)
-include $(BONUS_DEPS)

.PHONY: all clean fclean re help bonus
