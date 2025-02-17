#!/bin/bash

echo "Starting Linux environment preparation..."

# 检查是否在 Linux 上运行
if [ "$(uname)" != "Linux" ]; then
    echo "Error: This script should only be run on Linux"
    exit 1
fi

# 检查并安装必要的依赖
echo "Checking and installing dependencies..."
DEPS="gcc make xorg libxext-dev libbsd-dev libglfw3-dev cmake build-essential libx11-dev libgl1-mesa-dev pkg-config"
for dep in $DEPS; do
    if ! dpkg -l | grep -q "^ii  $dep"; then
        echo "Installing $dep..."
        sudo apt-get install -y "$dep"
    fi
done

# 备份 MLX 相关文件（如果存在）
if [ -d "mlx_mac" ]; then
    echo "Backing up mlx_mac directory..."
    mv mlx_mac mlx_mac_backup
fi

# 克隆 MLX42 库
echo "Setting up MLX42 for Linux..."
if [ ! -d "mlx_linux" ]; then
    echo "Cloning MLX42..."
    git clone https://github.com/codam-coding-college/MLX42.git mlx_linux
else
    echo "MLX42 already exists, updating..."
    cd mlx_linux
    git pull
    cd ..
fi

# 更新 Makefile 中的 MLX 路径
echo "Updating Makefile configuration..."
sed -i 's/mlx_mac/mlx_linux/g' Makefile
sed -i 's/-framework Cocoa -framework OpenGL -framework IOKit/-lGL -lX11 -lpthread -lXrandr -lXi -ldl/g' Makefile

# 清理并重新编译
echo "Cleaning and rebuilding project..."
make fclean
make re

echo "Linux environment preparation completed!"
echo "You can now run the game using: ./so_long maps/map.ber" 