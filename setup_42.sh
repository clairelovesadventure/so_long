#!/bin/bash

# 检测操作系统
OS="$(uname -s)"
case "${OS}" in
    Darwin*)    
        echo "🖥 Detected macOS system"
        BREW_PATH="/usr/local"  # 42 School 路径
        if [ -d "/opt/homebrew" ]; then
            BREW_PATH="/opt/homebrew"  # 个人 Mac 路径
        fi
        ;;
    Linux*)     
        echo "🐧 Detected Linux system"
        echo "Warning: This script is designed for 42 School's macOS environment"
        exit 1
        ;;
    *)          
        echo "❌ Unsupported operating system: ${OS}"
        exit 1
        ;;
esac

# 检查命令行参数
if [ "$1" ]; then
    BRANCH="$1"
    echo -e "${YELLOW}Using branch: $BRANCH${NC}"
else
    BRANCH="main"
    echo -e "${YELLOW}No branch specified, using default 'main' branch${NC}"
fi

echo "🚀 Starting 42 School environment setup..."

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# 检查函数
check_status() {
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ $1${NC}"
    else
        echo -e "${RED}✗ $1${NC}"
        exit 1
    fi
}

# 1. 检查 GLFW
echo -e "\n${YELLOW}Checking GLFW installation...${NC}"
if [ -f "${BREW_PATH}/lib/libglfw.dylib" ]; then
    echo -e "${GREEN}✓ GLFW is installed${NC}"
else
    echo -e "${YELLOW}Installing GLFW...${NC}"
    brew install glfw
    check_status "GLFW 安装"
fi

# 2. 检查必要目录
echo -e "\n${YELLOW}Checking project structure...${NC}"
for dir in "maps" "textures" "src" "includes" "libft"; do
    if [ -d "$dir" ]; then
        echo -e "${GREEN}✓ $dir directory exists${NC}"
    else
        echo -e "${RED}✗ $dir directory not found${NC}"
        exit 1
    fi
done

# 3. 清理并编译
echo -e "\n${YELLOW}Starting compilation...${NC}"
make fclean
check_status "Project cleanup"

make
check_status "Project compilation"

# 4. 设置权限
echo -e "\n${YELLOW}Setting execution permissions...${NC}"
chmod +x so_long
check_status "Permission setup"

# 5. 检查可执行文件
echo -e "\n${YELLOW}Verifying compilation...${NC}"
if [ -f "so_long" ]; then
    echo -e "${GREEN}✓ so_long executable created${NC}"
else
    echo -e "${RED}✗ Compilation failed${NC}"
    exit 1
fi

# 6. 检查地图文件
echo -e "\n${YELLOW}Checking map files...${NC}"
if [ -f "maps/map.ber" ]; then
    echo -e "${GREEN}✓ Default map file found${NC}"
else
    echo -e "${RED}✗ Default map file maps/map.ber not found${NC}"
    exit 1
fi

echo -e "\n${GREEN}Setup complete!${NC}"
echo -e "Run the game: ${YELLOW}./so_long maps/map.ber${NC}" 