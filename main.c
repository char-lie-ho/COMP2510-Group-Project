#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helpers.h"

int main() {
    FILE *inputFile, *outputFile;
    BITMAPFILEHEADER bmp_header;
    BITMAPV5HEADER dib_header;

    //TODO: use helper function to ask user the input and output file name
    char filename[] = "../image/cat.bmp";
    char output_filename[] = "../image/cat-new.bmp";

    // Open the BMP inputFile in binary mode
    inputFile = fopen(filename, "rb");
    if (!inputFile) {
        printf("Error opening inputFile %s\n", filename);
        fclose(inputFile);
        return 1;
    }

    // Obtain the Bitmap File header and DIB header(V5)
    if (fread(&bmp_header, sizeof(BITMAPFILEHEADER), 1, inputFile) != 1) {
        printf("Error reading BMP header\n");
        fclose(inputFile);
        return 1;
    }

    // Read the BITMAPV5HEADER structure
    if (fread(&dib_header, sizeof(BITMAPV5HEADER), 1, inputFile) != 1) {
        printf("Error reading DIB header\n");
        fclose(inputFile);
        return 1;
    }

    int width = dib_header.bV5Width;
    int height = dib_header.bV5Height;

    printf("width: %d, height: %d", width, height);

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL) {
        printf("Not enough memory to store image.\n");
        fclose(inputFile);
        return 1;
    }

    // Read image data
    for (int i = 0; i < height; i++) {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inputFile);

        // Skip over padding
        fseek(inputFile, padding, SEEK_CUR);
    }

    // TODO: Perform image processing operations here, using witch case

//    grayscale(height, width, image);
//    reflect(height, width, image);
//    blur(height, width, image);
//    sepia(height,  width, image);
//    brighten(height, width, image, -50);
    saturate(height, width, image, 50);
    // Close the input file
    fclose(inputFile);

    // Open the output BMP file in binary mode
    outputFile = fopen(output_filename, "wb");
    if (!outputFile) {
        printf("Error opening file %s\n", output_filename);
        fclose(outputFile);
        free(image);
        return 1;
    }

    // Write bmp header to output file
    fwrite(&bmp_header, sizeof(BITMAPFILEHEADER), 1, outputFile);

    // Write dib header to output file
    fwrite(&dib_header, sizeof(BITMAPV5HEADER), 1, outputFile);

    // Write the modified pixel data to the output file
    for (int i = 0; i < height; i++) {
        // Write row to output file
        fwrite(image[i], sizeof(RGBTRIPLE), width, outputFile);

        // Write padding at end of row
        for (int k = 0; k < padding; k++) {
            fputc(0x00, outputFile);
        }
    }

    // Close the output file
    fclose(outputFile);

    // Free memory for image
    free(image);

    return 0;
}
