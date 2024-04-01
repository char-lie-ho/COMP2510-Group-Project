#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE ** image);

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE ** image);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE ** image);

// Blur image
void blur(int height, int width, RGBTRIPLE ** image);

// Increase or decrease brightness
void brighten(int height, int width, RGBTRIPLE ** image, int unit);

// Increase or decrease saturation
void saturate(int height, int width, RGBTRIPLE ** image, int unit);
void thresholdFilter(int height, int width, RGBTRIPLE ** image, double threshold);
// Reflect image horizontally


// invert colour


// red / blue / green filter
// one interesting idea is to include all filter in the same, bottom 1/3 is red, middle is blue....


// combine 2 same size image together, image blend


// change brightness, saturation, hue


// rotation, can be tricky! have to alter the header as well

