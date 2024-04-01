#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helpers.h"



typedef struct {
    BITMAPFILEHEADER bmp_header;
    BITMAPV5HEADER dib_header;
    RGBTRIPLE ** image;
} bmp_data;

bmp_data read_bmp(char * file_name) {
    FILE *inputFile;
    bmp_data bmp = {0};

    // Open the BMP inputFile in binary mode
    inputFile = fopen(file_name, "rb");
    if (!inputFile) {
        printf("Error opening inputFile %s\n", file_name);
        fclose(inputFile);
        return bmp;
    }

    // Obtain the Bitmap File header and DIB header(V5)
    if (fread(&bmp.bmp_header, sizeof(BITMAPFILEHEADER), 1, inputFile) != 1) {
        printf("Error reading BMP header\n");
        fclose(inputFile);
        return bmp;
    }

    // Read the BITMAPV5HEADER structure
    if (fread(&bmp.dib_header, sizeof(BITMAPV5HEADER), 1, inputFile) != 1) {
        printf("Error reading DIB header\n");
        fclose(inputFile);
        return bmp;
    }

    int width = bmp.dib_header.bV5Width;
    int height = bmp.dib_header.bV5Height;

    printf("width: %d, height: %d", width, height);

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Allocate memory for image
    bmp.image = malloc(height * sizeof(RGBTRIPLE *));
    if (bmp.image == NULL) {
        printf("Not enough memory to store image.\n");
        fclose(inputFile);
        return bmp;
    }

    for (int i = 0; i < height; i++) {
        bmp.image[i] = malloc(width * sizeof(RGBTRIPLE));
        if (bmp.image[i] == NULL) {
            printf("Not enough memory to store image.\n");
            fclose(inputFile);
            while (--i >= 0) free(bmp.image[i]);
            free(bmp.image);
            return bmp;
        }
    }

    // Read image data
    for (int i = 0; i < height; i++) {
        // Read row into pixel array
        fread(bmp.image[i], sizeof(RGBTRIPLE), width, inputFile);

        // Skip over padding
        fseek(inputFile, padding, SEEK_CUR);
    }

    // Close the input file
    fclose(inputFile);

    return bmp;
}

void write_bmp(char * file_name, bmp_data bmp) {
    // Open the output BMP file in binary mode
    FILE * outputFile = fopen(file_name, "wb");
    if (!outputFile) {
        printf("Error opening file %s\n", file_name);
        fclose(outputFile);
    }

    // Write bmp header to output file
    fwrite(&bmp.bmp_header, sizeof(BITMAPFILEHEADER), 1, outputFile);

    // Write dib header to output file
    fwrite(&bmp.dib_header, sizeof(BITMAPV5HEADER), 1, outputFile);

    // Write the modified pixel data to the output file
    for (int i = 0; i < bmp.dib_header.bV5Height; i++) {
        // Write row to output file
        fwrite(bmp.image[i], sizeof(RGBTRIPLE), bmp.dib_header.bV5Width, outputFile);

        // Determine padding for scanlines
        int padding = (4 - (bmp.dib_header.bV5Width * sizeof(RGBTRIPLE)) % 4) % 4;
        // Write padding at end of row
        for (int k = 0; k < padding; k++) {
            fputc(0x00, outputFile);
        }
    }

    // Close the output file
    fclose(outputFile);
}

void free_bmp(bmp_data bmp)
{
    // Free memory for image
    for (int i = 0; i < bmp.dib_header.bV5Height; i++) {
        free(bmp.image[i]);
    }
    free(bmp.image);

}
int main()
{
    // TODO: Perform image processing operations here, using witch case

//    grayscale(height, width, image);
//    reflect(height, width, image);
//    sepia(height,  width, image);
//    brighten(height, width, image, -50);

    bmp_data bmp;

    bmp = read_bmp("../image/sky.bmp");
    blur(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image);
    write_bmp("../image/sky-blur.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/sky.bmp");
    thresholdFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, 0.5);
    write_bmp("../image/sky-thresholdFilter.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    brighten(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, 50);
    write_bmp("../image/cat-brighter.bmp", bmp);
    free_bmp(bmp);

    double RED[] = {255, 0, 0};
    double ORANGE[] = {255, 125, 0};
    double YELLOW[] = {255, 255, 0};
    double GREEN[] = {0, 255, 0};
    double CYAN[] = {0, 255, 255};
    double BLUE[] = {0, 125, 255};
    double PURPLE[] = {125, 0, 255};
    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, RED);
    write_bmp("../image/cat-RED.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, ORANGE);
    write_bmp("../image/cat-ORANGE.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, YELLOW);
    write_bmp("../image/cat-YELLOW.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, GREEN);
    write_bmp("../image/cat-GREEN.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, CYAN);
    write_bmp("../image/cat-CYAN.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, BLUE);
    write_bmp("../image/cat-BLUE.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    colorFilter(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, PURPLE);
    write_bmp("../image/cat-PURPLE.bmp", bmp);
    free_bmp(bmp);

    bmp = read_bmp("../image/cat.bmp");
    brighten(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, -50);
    write_bmp("../image/cat-dimmer.bmp", bmp);
    free_bmp(bmp);


    bmp = read_bmp("../image/cat.bmp");
    saturate(bmp.dib_header.bV5Height, bmp.dib_header.bV5Width, bmp.image, 20);
    write_bmp("../image/cat-saturater.bmp", bmp);
    free_bmp(bmp);

}