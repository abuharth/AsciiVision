// USAGE:
// a.exe <path-to-image> <width (characters)>

// TODO: give user option to use color or not

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGBColor;

typedef struct {
    double h;
    double s;
    double v;
} HSVColor;

unsigned char RGBToBrightness(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char luminance = 0.299*r + 0.587*g + 0.114*b;
    return luminance;
}

HSVColor rgb_to_hsv(unsigned char r, unsigned char g, unsigned char b) {
    double R = r/255.0f;
    double G = g/255.0f;
    double B = b/255.0f;

    double cmax = fmax(R, fmax(G, B));
    double cmin = fmin(R, fmin(G, B));
    double diff = cmax - cmin;
    double h = -1, s = -1;

    if (cmax == cmin) {
        h = 0;
    }
    else if (cmax == R) {
        h = fmod(60*((G - B)/diff) + 360, 360);
    }
    else if (cmax == G) {
        h = fmod(60*((B - R)/diff) + 120, 360);
    }
    else if (cmax == B) {
        h = fmod(60*((R - G)/diff) + 240, 360);
    }

    if (cmax == 0) {
        s = 0;
    }
    else {
        s = (diff/cmax)*100;
    }

    double v = cmax * 100;
    return (HSVColor){ h, s, v };
}

RGBColor hsv2rgb(float H, float S, float V) {
	float r, g, b;
	
	float h = H / 360;
	float s = S / 100;
	float v = V / 100;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}
	
	unsigned char R = r * 255;
	unsigned char G = g * 255;
	unsigned char B = b * 255;
    return (RGBColor){ R, G, B };
}


int main(int argc, char **argv) {
    int width, height, n;
    unsigned char *data = stbi_load(argv[1], &width, &height, &n, 3);
    if (!data) {
        printf("Failed to load sprite: \"%s\"\n", argv[1]);
        return -1;
    }

    int terminalWidth;
    if (argc >= 3) {
        terminalWidth = atoi(argv[2]);
        if (!terminalWidth) {
            printf("ERROR, input width is not a valid number");
            return -1;
        }
    }
    else {
        terminalWidth = 160;
    }

    int color = 1;
    if (argc == 4) {
        if (argv[3][0] == '-') {
            color = 0;
        }
    }

    if (width > terminalWidth/2) {
        int newWidth = terminalWidth/2;
        int newHeight = ((float)newWidth/(float)width) * height;
        unsigned char *tmp = stbir_resize_uint8_srgb(data, width, height, 0, 0, newWidth, newHeight, 0, STBIR_RGB);
        stbi_image_free(data);
        data = tmp;
        width = newWidth;
        height = newHeight;
    }

    unsigned char *luminances = (unsigned char*)malloc(width*height);

    char **colors = malloc(width*height*sizeof(char*));
    for (int i = 0; i < width*height; i++) {
        colors[i] = malloc(30 * sizeof(char));
        memset(colors[i], 0, 30);
    }

    int i, j;
    for (i = 0, j = 0; i < width*height*3; i += 3, j++) {
        unsigned char luminance = RGBToBrightness(data[i], data[i+1], data[i+2]);
        luminances[j] = luminance;

        if (color) {
            HSVColor hsv = rgb_to_hsv(data[i], data[i+1], data[i+2]);
            hsv.v = 100.0;
            RGBColor color = hsv2rgb(hsv.h, hsv.s, hsv.v);
            sprintf(colors[j], "\e[38;2;%d;%d;%dm", color.r, color.g, color.b);
        }
    }

    char *charMap = " .:;+coO8@";
    for (int i = 0; i < width*height; i++) {
        float lum = (float)luminances[i]/255.0f;
        int val = (lum*10 > 9)? 9 : lum*10;

        printf("%s%c%c", colors[i], charMap[val], charMap[val]);

        if ((i + 1) % width == 0) {
            printf("\n");
        }
    }

    printf("\e[0m");
    stbi_image_free(data);
    return 0;
}
