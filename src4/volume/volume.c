// volume.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// .wav header size (in bytes)
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check if cmd input is valid
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Read input file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open input file.\n");
        return 1;
    }

    // Write to output file
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open output file.\n");
        fclose(input);
        return 1;
    }

    // Convert factor input string to float
    float factor = atof(argv[3]);

    // Copy header 1st
    uint8_t header[HEADER_SIZE]; // create a storage for header
    fread(header, HEADER_SIZE, 1, input); // pointer to header, size of package, no. of packages, where to get
    fwrite(header, HEADER_SIZE, 1, output);

    // Create buffer for copying one sample at a time, size of 2 bytes (including signed integers)
    int16_t buffer;

    // Read samples from input file and write updated data to output file
    while (fread(&buffer, sizeof(int16_t), 1, input)) // read samples until return false
    {
        buffer *= factor; // apply factor to sample
        fwrite(&buffer, sizeof(int16_t), 1, output); // write sample package to output file
    }

    // Close files
    fclose(input);
    fclose(output);
}
