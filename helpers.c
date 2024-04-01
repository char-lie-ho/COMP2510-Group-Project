#include <malloc.h>
#include "helpers.h"
#include "math.h"
#include "stdio.h"

#define RED ((Color){255, 0, 0})
#define ORANGE {255, 165, 0}
#define YELLOW {255, 255, 0}
#define GREEN {0, 255, 0}
#define CYAN {0, 255, 255}
#define BLUE {0, 0, 255}
#define PURPLE {128, 0, 128}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE ** image)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int rgb = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = rgb;
            image[i][j].rgbtRed = rgb;
            image[i][j].rgbtGreen = rgb;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE ** image)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sR = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            int sG = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            int sB = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            if (sR > 255)
            {
                sR = 255;
            }
            if (sG > 255)
            {
                sG = 255;
            }
            if (sB > 255)
            {
                sB = 255;
            }
            image[i][j].rgbtBlue = sB;
            image[i][j].rgbtRed = sR;
            image[i][j].rgbtGreen = sG;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE ** image)
{
    // Iterate over each row in the image
    for (int i = 0; i < height; i++)
    {
        // Iterate over each column in the row, but only up to half the width
        for (int j = 0; j < width / 2; j++)
        {
            // Swap the pixels across the vertical axis (left and right)
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}


#include <stdio.h>

// Blur image
void blur(int height, int width, RGBTRIPLE ** image)
{
    int intensity;
    printf("Enter blur intensity (0-100): ");
    scanf("%d", &intensity);

    // Ensure the intensity is within the bounds
    if (intensity < 0) intensity = 0;
    if (intensity > 100) intensity = 100;

    // Map intensity to matrix size, e.g., 0 -> 1, 100 -> 9
    int matrixSize = 1 + (intensity / 12.5); // This maps 100 to 9, 50 to roughly 5, etc.
    int offset = matrixSize / 2; // Used to determine the range of surrounding pixels

    // Dynamically allocate memory for the temporary array
    RGBTRIPLE (*temp)[width] = malloc(height * sizeof(RGBTRIPLE[width]));
    if (temp == NULL)
    {
        printf("Error: Not enough memory to store temporary image.\n");
        return;
    }

    // Loop through each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            float totalR = 0, totalG = 0, totalB = 0;

            // Create a matrix around the current pixel based on the blur intensity
            for (int r = -offset; r <= offset; r++)
            {
                for (int c = -offset; c <= offset; c++)
                {
                    int curRow = i + r;
                    int curCol = j + c;

                    // Check if the current pixel is within bounds
                    if (curRow >= 0 && curRow < height && curCol >= 0 && curCol < width)
                    {
                        count++;
                        RGBTRIPLE pixel = image[curRow][curCol];
                        totalR += pixel.rgbtRed;
                        totalG += pixel.rgbtGreen;
                        totalB += pixel.rgbtBlue;
                    }
                }
            }

            // Calculate the average color values for the surrounding pixels
            temp[i][j].rgbtRed = round(totalR / count);
            temp[i][j].rgbtGreen = round(totalG / count);
            temp[i][j].rgbtBlue = round(totalB / count);
        }
    }

    // Copy the blurred image from temp to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    // Free the memory allocated for the temp image
    free(temp);
}

BYTE constraint(int value)
{
    if (value < 0) {
        return 0;
    } else if (value > 255) {
        return 255;
    } else {
        return value;
    }
}

RGBTRIPLE generate_rgb(int red, int green, int blue)
{
    RGBTRIPLE rgb;
    rgb.rgbtRed = constraint(red);
    rgb.rgbtGreen = constraint(green);
    rgb.rgbtBlue = constraint(blue);
    return rgb;
}

void brighten(int height, int width, RGBTRIPLE ** image, int unit)
{
    // Loop through each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = generate_rgb((int)image[i][j].rgbtRed + unit,
                                      (int)image[i][j].rgbtGreen + unit,
                                      (int)image[i][j].rgbtBlue + unit);
        }
    }
}

void saturate(int height, int width, RGBTRIPLE ** image, int unit)
{
    // Loop through each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = generate_rgb((int)image[i][j].rgbtRed * (100 + unit) / 100,
                                       (int)image[i][j].rgbtGreen * (100 + unit) / 100,
                                       (int)image[i][j].rgbtBlue * (100 + unit) / 100);
        }
    }
}

//threshold is a double from 0.0 to 1.0
void thresholdFilter(int height, int width, RGBTRIPLE ** image, double threshold){
    double rbgThreshold = 255 * threshold;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int rgb = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            if (rgb < rbgThreshold) {
                image[i][j].rgbtBlue = 0;
                image[i][j].rgbtRed = 0;
                image[i][j].rgbtGreen = 0;
            } else {
                image[i][j].rgbtBlue = 255;
                image[i][j].rgbtRed = 255;
                image[i][j].rgbtGreen = 255;
            }

        }
    }
}

//colorRGB is a double array with 3 elements, representing 3 rgb colors
void colorFilter(int height, int width, RGBTRIPLE **image, double const colorRGB[])
{
    double redFactor = 1 + colorRGB[0] / 255;
    double greenFactor = 1 + colorRGB[1] / 255;
    double blueFactor = 1 + colorRGB[2] / 255;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int newRed = (int)(image[i][j].rgbtRed * redFactor);
            int newGreen = (int)(image[i][j].rgbtGreen * greenFactor);
            int newBlue = (int)(image[i][j].rgbtBlue * blueFactor);
            image[i][j].rgbtRed = (BYTE)constraint(newRed);
            image[i][j].rgbtGreen = (BYTE)constraint(newGreen);
            image[i][j].rgbtBlue = (BYTE)constraint(newBlue);
        }
    }
}
