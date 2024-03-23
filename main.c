#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helpers.h"

int main() {
    FILE *inputFile, *outputFile;
    BITMAPFILEHEADER bmp_header;
    BITMAPV5HEADER dib_header;
    uint8_t *pixels = NULL;

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

    // Determine padding for scanlines
    int padding = (4 - (dib_header.bV5Width * sizeof(RGBTRIPLE)) % 4) % 4;


    RGBTRIPLE(*image)[dib_header.bV5Width] = calloc(dib_header.bV5Height, dib_header.bV5Width * sizeof(RGBTRIPLE));
    if (image == NULL) {
        printf("Not enough memory to store image.\n");
        fclose(inputFile);
        free(image);
        return 7;
    }

    // Iterate over infile's scanlines
    for (int i = 0; i < dib_header.bV5Height; i++) {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), dib_header.bV5Width, inputFile);

        // Skip over padding
        fseek(inputFile, padding, SEEK_CUR);
    }

    //TODO- update the method selection, using witch case
    grayscale(dib_header.bV5Height, dib_header.bV5Width, image);
//    sepia(dib_header.bV5Height,  dib_header.bV5Width, image);

    // Close the input inputFile
    fclose(inputFile);

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
    for (int i = 0; i < dib_header.bV5Height; i++) {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), dib_header.bV5Width, outputFile);

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
