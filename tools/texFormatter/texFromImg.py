#! /usr/bin/python3

# this is a simple script that will take a picture (any format)
# as input and create a .tex (my own texture-format) file

# .tex will contain:
# header:
#   2bytes width 2bytes height
#   byte that stores which color-format (RGB 16bit 32bit etc)
# Pixel info:
#   depending on the color format the bytes describing each pixel
#   from left to right top to bottom (like reading this comment)

from sys import argv            # to use arguments
from os import path             # to validate existing file
from PIL import Image           # to load and get data from images

# known pixel-formats
# not yet implemented!!
pixelFormats = [
    "16",
    "32"
]

# argv[1]: path/to/file (relative to the directory of execution ".")
# argv[2]: color-mode (16Bit, 32Bit etc.)
# argv[3]: output name

# checking for correct arguments

# are there 3 arguments?
if len(argv) is not 4:
    print("ERROR: Wrong number of arguments")
    exit()

# checking if file exists
if path.isfile(argv[1]) is False:
    print("ERROR: File", argv[1], "doesnt exist")
    exit()

# checking if pixel format exists
if argv[2] not in pixelFormats:
    print("ERROR: Format", argv[2], "doesnt exist")
    exit()

# checking if output-name already exists
if path.isfile(argv[3]) is True:
    print(argv[3], "already exists. Overwrite? y/n")

    userInput = ""

    while userInput.lower() != "y" and userInput.lower() != "n":
        userInput = input()

    if userInput.lower() == "n":
        exit()


image = Image.open(argv[1])         # opens image-file
rgb_im = image.convert("RGBA")      # converts image to rgb-file
texture = open(argv[3], "wb")       # creates output-file with given name

# printing what script will do...
print("Converting", argv[1])
print("Format:", argv[2], "\b-Bit Color")
print("Dimensions:", image.size)

# saves dimensions in four bytes
texture.write((image.size[0]).to_bytes(2,byteorder="big"))
texture.write((image.size[1]).to_bytes(2,byteorder="big"))
# saves encoded color-format
# texture.write((int(argv[2])).to_bytes(1,byteorder="big"))

# go through all pixels and write them to the file
for y in range(image.size[1]):
    for x in range(image.size[0]):
        r, g, b, a = rgb_im.getpixel((x, y))
        #print((x,y), r, g, b, a)
        texture.write((r).to_bytes(1,byteorder="big"))
        texture.write((g).to_bytes(1,byteorder="big"))
        texture.write((b).to_bytes(1,byteorder="big"))
        texture.write((a).to_bytes(1,byteorder="big"))

# closing all opened data
rgb_im.close()
image.close()
texture.close()
