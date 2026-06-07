# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 21:03:44 by baelgadi          #+#    #+#              #
#    Updated: 2026/02/20 06:49:29 by baelgadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ─────────────────────────────────
#             🎨 COLORS
# ─────────────────────────────────

RESET = \033[0m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
GRAY = \033[90m
UNDERLINE = \033[4m

# ─────────────────────────────────
#         🛠️ CONFIGURATION
# ─────────────────────────────────

NAME = libft.a

CONVERT =	ft_atoi	\
			ft_itoa	\
			ft_atof	\
			ft_atoll

CTYPE =	ft_isalnum	\
		ft_isalpha	\
		ft_isascii	\
		ft_isdigit	\
		ft_isprint	\
		ft_tolower	\
		ft_toupper	\
		ft_isspace

IO =	ft_putchar_fd	\
		ft_putendl_fd	\
		ft_putnbr_fd	\
		ft_putstr_fd	\
		ft_putstr_bin_fd

MEMORY = ft_bzero	\
		ft_calloc	\
		ft_memchr	\
		ft_memcmp	\
		ft_memcpy	\
		ft_memmove	\
		ft_memset

STRING = ft_split			\
		ft_strchr			\
		ft_strdup			\
		ft_striteri			\
		ft_strjoin			\
		ft_strlcat			\
		ft_strlcpy			\
		ft_strlen			\
		ft_strmapi			\
		ft_strncmp			\
		ft_strncasecmp		\
		ft_strnstr			\
		ft_strrchr			\
		ft_strtrim			\
		ft_substr			\
		ft_free_strarray	\
		ft_strjoin_and_free \
		ft_str_is_numeric	\
		ft_arrlen

LISTS =	ft_lstadd_back	\
		ft_lstadd_front	\
		ft_lstclear		\
		ft_lstdelone	\
		ft_lstiter		\
		ft_lstlast		\
		ft_lstmap		\
		ft_lstnew		\
		ft_lstsize

FT_PRINTF =	ft_printf	\
			ft_printf_utils	\
			ft_printf_hex

GNL =	get_next_line	\
		get_next_line_utils

# ─────────────────────────────────
#      🚧 SETUP & DEPENDENCIES
# ─────────────────────────────────

SRCS = $(addprefix src/convert/, $(addsuffix .c,$(CONVERT))) \
	$(addprefix src/ctype/, $(addsuffix .c,$(CTYPE))) \
	$(addprefix src/io/, $(addsuffix .c,$(IO))) \
	$(addprefix src/memory/, $(addsuffix .c,$(MEMORY))) \
	$(addprefix src/string/, $(addsuffix .c,$(STRING))) \
	$(addprefix src/lists/, $(addsuffix .c,$(LISTS))) \
	$(addprefix src/ft_printf/, $(addsuffix .c,$(FT_PRINTF))) \
	$(addprefix src/gnl/, $(addsuffix .c,$(GNL)))

OBJS = ${SRCS:.c=.o}

DEPS =  $(OBJS:.o=.d)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I ./include -MMD -MP
AR = ar
ARFLAGS = rcs

# ─────────────────────────────────
#        💽 COMPILATION RULE
# ─────────────────────────────────

.c.o:
	@printf "Compiling $(BLUE)%-35s$(RESET)" $<
	@$(CC) $(CFLAGS) -c $< -o ${<:.c=.o} && printf "✅\n" || printf "❌\n"

# ─────────────────────────────────
#            🎯 TARGETS
# ─────────────────────────────────

all:	$(NAME)

$(NAME):	$(OBJS)
	@echo "$(MAGENTA)⏳ Archiving to $(NAME)$(RESET)"
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS)
	@echo "$(GREEN)✅ $(NAME) created$(RESET)"

# ─────────────────────────────────
#            🧹 CLEANING
# ─────────────────────────────────

clean:
	@rm -f $(OBJS) $(OBJS:.o=.d)
	@echo "$(YELLOW)🧹 Libft objects and dependencies deleted (clean)$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)❌ $(NAME) removed (fclean)$(RESET)"

re:	fclean all

# ─────────────────────────────────
#         🌐 HELP & FOOTER
# ─────────────────────────────────

help:
	@echo "$(BLUE)Available targets:$(RESET)"
	@echo "$(YELLOW)all      $(RESET)– Build libft.a"
	@echo "$(YELLOW)clean    $(RESET)– Delete .o files"
	@echo "$(YELLOW)fclean   $(RESET)– Full clean (including libft.a)"
	@echo "$(YELLOW)re       $(RESET)– fclean + all"

-include $(DEPS)

.PHONY: all clean fclean re bonus
