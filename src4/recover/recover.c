// recover.c
// okay so this program recovers JPEGs from a forensic image (card.raw etc)
// read 512 bytes blocks, find start of jpeg, save into separate files

#include <stdio.h>   // for FILE, fopen, fread, fwrite, fclose, printf
#include <stdlib.h>  // for exit codes
#include <stdint.h>  // for uint8_t (unsigned char basically)
#include <stdbool.h> // for bool, true, false

int main(int argc, char *argv[])
{
    // argc = argument count, argv = argument vector (array of strings)
    // we want exactly 2 args (program name + file name)
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n"); // tell user how to run program
        return 1; // 1 = error code
    }

    // open the file user passed (argv[1] is the second argument)
    FILE *inptr = fopen(argv[1], "r"); // "r" = read mode
    if (inptr == NULL) // if cannot open (file not found or permission denied)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    // define a BYTE type = just makes it easier to read (uint8_t = 1 byte)
    typedef uint8_t BYTE;

    BYTE buffer[512]; // array of 512 bytes (our "block" of data)
    FILE *outptr = NULL; // pointer for output JPEG file (start NULL = none open yet)
    char filename[8]; // stores something like "000.jpg\0" (needs space for 7 chars + null)
    int file_count = 0; // keeps track of how many jpgs we made
    bool found_jpeg = false; // flag = are we currently writing a jpg?

    size_t bytes_read;
    // fread returns how many "items" read, here we ask for bytes so it returns bytes count
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), inptr)) > 0)
    {
        // check if this 512 block starts with JPEG signature
        // first 3 bytes must be: 0xff 0xd8 0xff
        // 4th byte is like 0xe0, 0xe1, 0xe2 ... 0xef (0xf0 mask)
        bool is_jpeg_header = (buffer[0] == 0xff &&
                               buffer[1] == 0xd8 &&
                               buffer[2] == 0xff &&
                               (buffer[3] & 0xf0) == 0xe0);
        // note: (buffer[3] & 0xf0) == 0xe0 means take top 4 bits only and compare

        if (is_jpeg_header)
        {
            // if we already writing one jpeg, close it first (because new jpeg started)
            if (found_jpeg)
            {
                fclose(outptr);
            }
            else
            {
                found_jpeg = true; // first jpeg we found -> now writing mode
            }

            // create new filename -> example "000.jpg", "001.jpg", etc.
            sprintf(filename, "%03d.jpg", file_count);
            // %03d = pad number to 3 digits with leading zeros (so 0 -> 000)
            file_count++; // next time use next number

            // open new file for writing
            outptr = fopen(filename, "w");
            if (outptr == NULL)
            {
                fprintf(stderr, "Could not create %s.\n", filename);
                fclose(inptr); // close input before exiting
                return 1;
            }

            // write current block (512 bytes) into the file
            fwrite(buffer, 1, bytes_read, outptr);
        }
        else if (found_jpeg)
        {
            // if this block is NOT a jpeg header but we are in the middle of writing
            // -> just keep writing more data to current jpeg file
            fwrite(buffer, 1, bytes_read, outptr);
        }
        // else: no jpeg found yet and not writing -> skip the data
    }

    // after loop end: close output file if we have one
    if (outptr != NULL)
    {
        fclose(outptr);
    }
    fclose(inptr);

    return 0; // 0 = program success
}
