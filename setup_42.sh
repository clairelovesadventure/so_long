#!/bin/bash

echo "🚀 开始设置 42 School 环境..."

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
echo -e "\n${YELLOW}检查 GLFW 安装...${NC}"
if brew list | grep -q glfw; then
    echo -e "${GREEN}✓ GLFW 已安装${NC}"
else
    echo -e "${YELLOW}正在安装 GLFW...${NC}"
    brew install glfw
    check_status "GLFW 安装"
fi

# 2. 检查必要目录
echo -e "\n${YELLOW}检查项目结构...${NC}"
for dir in "maps" "textures" "src" "includes" "libft"; do
    if [ -d "$dir" ]; then
        echo -e "${GREEN}✓ $dir 目录存在${NC}"
    else
        echo -e "${RED}✗ $dir 目录不存在${NC}"
        exit 1
    fi
done

# 3. 清理并编译
echo -e "\n${YELLOW}开始编译项目...${NC}"
make fclean
check_status "清理项目"

make
check_status "编译项目"

# 4. 设置权限
echo -e "\n${YELLOW}设置执行权限...${NC}"
chmod +x so_long
check_status "设置权限"

# 5. 检查可执行文件
echo -e "\n${YELLOW}验证编译结果...${NC}"
if [ -f "so_long" ]; then
    echo -e "${GREEN}✓ so_long 可执行文件已创建${NC}"
else
    echo -e "${RED}✗ 编译失败${NC}"
    exit 1
fi

# 6. 检查地图文件
echo -e "\n${YELLOW}检查地图文件...${NC}"
if [ -f "maps/map.ber" ]; then
    echo -e "${GREEN}✓ 找到默认地图文件${NC}"
else
    echo -e "${RED}✗ 未找到默认地图文件 maps/map.ber${NC}"
    exit 1
fi

echo -e "\n${GREEN}设置完成！${NC}"
echo -e "运行游戏: ${YELLOW}./so_long maps/map.ber${NC}" 