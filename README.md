# ASCII Vision
A simple program that converts images to ASCII art

# Example
![image2ascii](https://github.com/user-attachments/assets/c0acfeae-5ac1-4427-8215-3f640756c98c)

# Dependencies
stb_image.h [(link)](https://github.com/nothings/stb/blob/master/stb_image.h)

stb_image_resize2.h [(link)](https://github.com/nothings/stb/blob/master/stb_image_resize2.h)

# Compilation
`gcc main.c -lm`

# Usage
`a.exe <path/to/image> <width to render> <flag: optional>`

Width parameter will represent how ascii pixels the printed image will be (1 ascii pixel is 2 characters wide)

The only flag currently available is -nc which will print to terminal without color
