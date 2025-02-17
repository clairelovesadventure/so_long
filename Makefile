NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./includes -I./mlx_mac/include -I./libft
LIBS = -L./libft -lft

# 源文件
SRCS = src/main.c src/map_parser.c src/game.c src/graphics.c src/utils.c \
src/animation.c
OBJS = $(SRCS:src/%.c=obj/%.o)

# MLX42 设置
MLX42_PATH = mlx_mac
MLX42_BUILD = $(MLX42_PATH)/build
MLX42_LIB = $(MLX42_BUILD)/libmlx42.a
LIBS += -L$(MLX42_BUILD) -lmlx42 -L/opt/homebrew/lib -lglfw -framework Cocoa -framework OpenGL -framework IOKit

all: obj $(MLX42_LIB) libft/libft.a $(NAME)

obj:
	mkdir -p obj

$(MLX42_LIB):
	@cmake $(MLX42_PATH) -B $(MLX42_BUILD)
	@make -C $(MLX42_BUILD) -j4

libft/libft.a:
	@make -C libft

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj
	make -C libft clean
	rm -rf $(MLX42_BUILD)

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re 