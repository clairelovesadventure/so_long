NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -fsanitize=address

# 自动检测操作系统
UNAME_S := $(shell uname -s)

# MacOS 设置
ifeq ($(UNAME_S),Darwin)
    MLX42_PATH = mlx_mac
    INCLUDES = -I./includes -I$(MLX42_PATH)/include -I./libft

    # 基础库
    LIBS = -L./libft -lft
    LIBS += -L$(MLX42_BUILD) -lmlx42
    
    # 检查并安装 GLFW
    ifeq ($(shell which brew),)
        $(error Homebrew is not installed. Please install Homebrew first)
    endif
    
    ifeq ($(shell brew list | grep glfw),)
        $(shell brew install glfw)
    endif
    
    # 优先检查 homebrew 路径
    ifeq ($(shell test -d /opt/homebrew/lib && echo yes),yes)
        LIBS += -L/opt/homebrew/lib -lglfw
    else ifeq ($(shell test -d /usr/local/lib && echo yes),yes)
        LIBS += -L/usr/local/lib -lglfw
    endif

    # macOS 框架
    LIBS += -framework Cocoa -framework OpenGL -framework IOKit
else
    LDFLAGS += -static-libsan
endif

# Linux 设置
ifeq ($(UNAME_S),Linux)
    MLX42_PATH = mlx_linux
    INCLUDES = -I./includes -I./mlx_linux/include -I./libft
    LIBS = -L./libft -lft
    LIBS += -L$(MLX42_BUILD) -lmlx42 -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

# 源文件
SRCS = src/main.c src/map_parser.c src/game.c src/graphics.c src/utils.c \
src/animation.c src/game_cleanup.c src/game_movement.c src/game_images.c \
src/map_validation.c src/map_utils.c src/game_init.c src/map_path.c src/render.c
OBJS = $(SRCS:src/%.c=obj/%.o)

# MLX42 设置
MLX42_BUILD = $(MLX42_PATH)/build
MLX42_LIB = $(MLX42_BUILD)/libmlx42.a

all: obj $(MLX42_LIB) libft/libft.a $(NAME)

obj:
	mkdir -p obj

$(MLX42_LIB):
	@if [ ! -d $(MLX42_PATH) ]; then \
		git clone --depth=1 https://github.com/codam-coding-college/MLX42.git $(MLX42_PATH); \
	fi
	@cmake -B $(MLX42_BUILD) $(MLX42_PATH)
	@cmake --build $(MLX42_BUILD) -j4

libft/libft.a:
	@make -C libft

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)

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