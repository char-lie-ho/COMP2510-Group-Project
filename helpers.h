#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Increase or decrease brightness
void brighten(int height, int width, RGBTRIPLE image[height][width], int unit);

// Reflect image horizontally


// invert colour


// red / blue / green filter
// one interesting idea is to include all filter in the same, bottom 1/3 is red, middle is blue....


// combine 2 same size image together, image blend


// change brightness, saturation, hue


// rotation, can be tricky! have to alter the header as well

