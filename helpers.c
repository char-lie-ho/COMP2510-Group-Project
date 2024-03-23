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
    return;
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
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    puts("r");
    for (int i = 0; i < height; i++)
    {
        //change the j to "width-j"
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp[height][width];
            temp[i][j] = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
//loop throught each pixel in the original image
//create a 3*3 matrix and a temp image
//count how many neighbour pixel in the matrix
//sum each color channel and divided by count
//replace the temp image with the original image
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            int rowCoor[] = {i - 1, i, i + 1};
            int colCoor[] = {j - 1, j, j + 1};
            float totalR = 0, totalG = 0, totalB = 0;

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int curRow = rowCoor[r];
                    int curCol = colCoor[c];
                    if (curRow >= 0 && curRow < height && curCol >= 0 && curCol < width) //check if it is inbound
                    {
                        count++;
                        RGBTRIPLE pixel = image[curRow][curCol];
                        totalR += pixel.rgbtRed;
                        totalG += pixel.rgbtGreen;
                        totalB += pixel.rgbtBlue;
                    }

                    temp[i][j].rgbtRed = round(totalR / count);
                    temp[i][j].rgbtGreen = round(totalG / count);
                    temp[i][j].rgbtBlue = round(totalB / count);
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
