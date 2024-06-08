from PIL import Image


def convert_png_to_tga(png_file, tga_file):
    try:
        # 打开PNG图像
        png_image = Image.open(png_file)

        # 将PNG图像转换为TGA图像
        png_image.save(tga_file)

        print(f"PNG图像 '{png_file}' 已成功转换为TGA图像 '{tga_file}'")
    except Exception as e:
        print(f"转换失败: {e}")



# 指定PNG和TGA文件的路径
png_filename = "C:\\Users\\Lingyin\\CLionProjects\\aSmallRender\\obj\\pic.png"
tga_filename = "C:\\Users\\Lingyin\\CLionProjects\\aSmallRender\\obj\\pic.tga"
png_filename = input()
tga_filename = input()
# 将PNG转换为TGA
convert_png_to_tga(png_filename, tga_filename)