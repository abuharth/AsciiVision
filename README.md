# ASCII Vision
A simple program that transforms images to ASCII art

# Example
![image2ascii](https://github.com/user-attachments/assets/caf5f121-3ee0-45f4-9f0a-73eb2e283939)

# Dependencies
stb_image.h [(link)](https://github.com/nothings/stb/blob/master/stb_image.h)

stb_image_resize2.h [(link)](https://github.com/nothings/stb/blob/master/stb_image_resize2.h)

# Compilation
`gcc main.c -lm`

# Usage
`a.exe <path/to/image> <width> <flags>`

width: width of ascii art (1 ascii pixel is 2 characters wide)

flags: -nc (print to terminal without color)

#### ::NOTE:: Terminal must support ANSI for colors to work
