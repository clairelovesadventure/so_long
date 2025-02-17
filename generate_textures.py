import sys
import os
from PIL import Image, ImageDraw

def ensure_directory_exists():
    try:
        if not os.path.exists('textures'):
            os.makedirs('textures')
    except Exception as e:
        print(f"Error creating textures directory: {e}")
        sys.exit(1)

def save_image(img, path):
    try:
        img.save(path)
    except Exception as e:
        print(f"Error saving {path}: {e}")
        sys.exit(1)

def create_player():
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 身体（圆形）
    draw.ellipse([4, 4, 28, 28], fill=(255, 100, 100), outline=(200, 50, 50), width=2)
    
    # 眼睛
    draw.ellipse([10, 12, 14, 16], fill=(255, 255, 255))
    draw.ellipse([18, 12, 22, 16], fill=(255, 255, 255))
    
    # 笑容
    draw.arc([12, 14, 20, 22], 0, 180, fill=(255, 255, 255), width=2)
    
    img.save('textures/player.png')

def create_wall():
    try:
        img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
        draw = ImageDraw.Draw(img)
        
        # 砖块背景
        draw.rectangle([0, 0, 31, 31], fill=(120, 110, 100))
        
        # 砖块纹理
        for y in range(0, 32, 8):
            for x in range(0, 32, 16):
                offset = 8 if y % 16 else 0
                draw.rectangle([x + offset, y, x + offset + 14, y + 6], 
                             fill=(140, 130, 120), outline=(100, 90, 80))
        
        save_image(img, 'textures/wall.png')
    except Exception as e:
        print(f"Error creating wall texture: {e}")
        sys.exit(1)

def create_collectible():
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 金币效果
    draw.ellipse([4, 4, 28, 28], fill=(255, 215, 0))  # 外圈
    draw.ellipse([8, 8, 24, 24], fill=(255, 200, 0))  # 内圈
    
    # 闪光效果
    draw.line([16, 2, 16, 6], fill=(255, 255, 200), width=2)
    draw.line([16, 26, 16, 30], fill=(255, 255, 200), width=2)
    draw.line([2, 16, 6, 16], fill=(255, 255, 200), width=2)
    draw.line([26, 16, 30, 16], fill=(255, 255, 200), width=2)
    
    img.save('textures/collectible.png')

def create_exit():
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 门框
    draw.rectangle([4, 0, 28, 31], fill=(150, 100, 50))
    draw.rectangle([6, 2, 26, 29], fill=(180, 130, 80))
    
    # 门把手
    draw.ellipse([18, 14, 22, 18], fill=(220, 180, 0))
    draw.ellipse([19, 15, 21, 17], fill=(255, 215, 0))
    
    # 木纹
    for y in range(4, 28, 6):
        draw.line([8, y, 24, y], fill=(130, 80, 30), width=1)
    
    img.save('textures/exit.png')

def create_floor():
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 基础地板颜色
    draw.rectangle([0, 0, 31, 31], fill=(200, 200, 190))
    
    # 添加纹理
    for i in range(0, 32, 8):
        for j in range(0, 32, 8):
            if (i + j) % 16 == 0:
                draw.rectangle([i, j, i+7, j+7], 
                             fill=(190, 190, 180),
                             outline=(180, 180, 170))
    
    img.save('textures/floor.png')

def create_enemy():
    img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 身体（幽灵形状）
    draw.ellipse([4, 4, 28, 20], fill=(150, 0, 200))  # 头部
    points = [(4, 16), (4, 24), (10, 28), (16, 24), 
              (22, 28), (28, 24), (28, 16)]
    draw.polygon(points, fill=(150, 0, 200))  # 飘动的下摆
    
    # 眼睛（生气的表情）
    draw.ellipse([10, 8, 15, 13], fill=(255, 255, 255))
    draw.ellipse([19, 8, 24, 13], fill=(255, 255, 255))
    draw.ellipse([11, 9, 14, 12], fill=(255, 0, 0))
    draw.ellipse([20, 9, 23, 12], fill=(255, 0, 0))
    
    img.save('textures/enemy.png')

def create_player_frames():
    try:
        for i in range(4):
            img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            draw = ImageDraw.Draw(img)
            
            offset = i * 1
            draw.ellipse([4, 4 + offset, 27, 27 + offset], 
                        fill=(255, 100, 100), outline=(200, 50, 50), width=2)
            
            draw.ellipse([10, 10 + offset, 14, 14 + offset], fill=(255, 255, 255))
            draw.ellipse([18, 10 + offset, 22, 14 + offset], fill=(255, 255, 255))
            
            draw.arc([12, 16 + offset, 20, 20 + offset], 0, 180, fill=(255, 255, 255), width=2)
            
            save_image(img, f'textures/player_{i}.png')
    except Exception as e:
        print(f"Error creating player frames: {e}")
        sys.exit(1)

def create_collectible_frames():
    try:
        for i in range(8):
            img = Image.new('RGBA', (32, 32), (0, 0, 0, 0))
            draw = ImageDraw.Draw(img)
            
            angle = i * 45
            
            # 金币效果
            draw.ellipse([4, 4, 28, 28], fill=(255, 215, 0))
            draw.ellipse([8, 8, 24, 24], fill=(255, 200, 0))
            
            # 旋转的闪光效果
            from math import sin, cos, radians
            rad = radians(angle)
            cx, cy = 16, 16
            length = 12
            
            for a in range(0, 360, 90):
                rad_a = radians(a + angle)
                x1 = cx + length * cos(rad_a)
                y1 = cy + length * sin(rad_a)
                x2 = cx + (length/2) * cos(rad_a)
                y2 = cy + (length/2) * sin(rad_a)
                draw.line([x2, y2, x1, y1], fill=(255, 255, 200), width=2)
            
            save_image(img, f'textures/collectible_{i}.png')
    except Exception as e:
        print(f"Error creating collectible frames: {e}")
        sys.exit(1)

def check_image_size(img, name):
    """检查图像大小是否符合要求"""
    if img.size != (32, 32):
        print(f"Error: {name} size must be 32x32 pixels")
        sys.exit(1)

def main():
    try:
        ensure_directory_exists()
        
        # 生成所有纹理
        create_player_frames()
        create_wall()
        create_collectible_frames()
        create_exit()
        create_floor()
        create_enemy()
        print("Textures generated successfully!")
    except KeyboardInterrupt:
        print("\nTexture generation interrupted by user")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error during texture generation: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main() 