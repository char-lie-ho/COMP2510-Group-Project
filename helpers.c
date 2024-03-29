#include <malloc.h>
#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
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
void sepia(int height, int width, RGBTRIPLE image[height][width])
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
void reflect(int height, int width, RGBTRIPLE image[height][width])
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


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Dynamically allocate memory for the temp array, have to use malloc to prevent stack overflow
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

            // Create a 3x3 matrix around the current pixel
            // Change the for loop bound below to change the range of blur
            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
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

void brighten(int height, int width, RGBTRIPLE image[height][width], int unit)
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

void saturate(int height, int width, RGBTRIPLE image[height][width], int unit)
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
