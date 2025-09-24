import os, sys
from math import ceil, floor
from PIL import Image, ImageDraw, ImageColor, ImageFile

def get_jpeg(dir_path):
    jpeg = ""
    for filename in os.listdir(dir_path):
        if filename.lower().endswith(".jpg") or filename.lower().endswith(".jpeg"):
            full_path = os.path.join(dir_path, filename)
            if os.path.isfile(full_path):
                jpeg = full_path
    if jpeg == "":
        print("No JPG image detected. Closing Program.")
        sys.exit()
    return jpeg


dir_path = os.getcwd()

im = Image.open(get_jpeg(dir_path)) 

charASCII = "@$B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1}{[]?-_+~<>i!lI;:,\"^`'. "  # Darkest char to brightest char
File_object = open(dir_path + "\\text.txt", "w+") # Text file
pixels = list(im.getdata())
width, height = im.size
ratio = 255/(len(charASCII)-1)
matrix = []
i = 0

for y in range(height):
    sOne = ''.join(matrix)
    sTwo = '\n'
    L = [sOne, sTwo]
    File_object.writelines(L)
    #print(sOne)
    matrix = []
    for x in range(width):
        brightness = ceil((pixels[i][0]+pixels[i][1]+pixels[i][2])/3)
        #for j in range(2):         #double text image size
        matrix.append(charASCII[floor(brightness/ratio)])
        i += 1

File_object.close()
print('Done!')