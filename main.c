#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helpers.h"

int main() {
    FILE *inputFile, *outputFile;
    BITMAPFILEHEADER bmp_header;
    BITMAPV5HEADER dib_header;
    uint8_t *pixels = NULL;
    uint32_t row_size;
    uint32_t padding_size;
    uint32_t padded_row_size;

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

    // Calculate row size in bytes (including padding)
    row_size = dib_header.bV5Width * 3;
    padding_size = (4 - (row_size % 4)) % 4;
    padded_row_size = row_size + padding_size;

    // Allocate memory for pixel data
    size_t pixel_count = dib_header.bV5Height * padded_row_size;
    pixels = (uint8_t *) malloc(pixel_count);
    if (!pixels) {
        printf("Error allocating memory\n");
        fclose(inputFile);
        return 1;
    }

    // Read pixel data
    if (fread(pixels, sizeof(uint8_t), pixel_count, inputFile) != pixel_count) {
        printf("Error reading pixel data\n");
        fclose(inputFile);
        free(pixels);
        return 1;
    }

    // Close the input inputFile
    fclose(inputFile);

    // Convert to grayscale
    for (size_t i = 0; i < pixel_count; i += 3) {
        // Grayscale value is the average of the RGB components
        uint8_t r = pixels[i];
        uint8_t g = pixels[i + 1];
        uint8_t b = pixels[i + 2];
        uint8_t grayscale = (uint8_t) ((r + g + b) / 3);
        pixels[i] = grayscale;
        pixels[i + 1] = grayscale;
        pixels[i + 2] = grayscale;
    }

    // Open the output BMP file in binary mode
    outputFile = fopen(output_filename, "wb");
    if (!outputFile) {
        printf("Error opening file %s\n", output_filename);
        free(pixels);
        return 1;
    }

    // Write bmp header to output file
    fwrite(&bmp_header, sizeof(BITMAPFILEHEADER), 1, outputFile);

    // Write dib header to output file
    fwrite(&dib_header, sizeof(BITMAPV5HEADER), 1, outputFile);


    // Write the modified pixel data to the output file
    if (fwrite(pixels, sizeof(uint8_t), pixel_count, outputFile) != pixel_count) {
        printf("Error writing pixel data\n");
        fclose(outputFile);
        free(pixels);
        return 1;
    }

    // Close the output file
    fclose(outputFile);

    // Free allocated memory
    free(pixels);

    return 0;
}
