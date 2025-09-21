#include "helpers.h"
#include <math.h> // for float and round
#include <stdlib.h>  // for calloc and free

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            // Average and round
            int average = (int) round((red + green + blue) / 3.0);

            // Set all three channels to the average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image (box blur)
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the image to store blurred results
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (copy == NULL)
    {
        return; // fail gracefully if memory allocation fails
    }

    // Iterate over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Loop over 3x3 box centered at (i, j)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check boundaries
                    if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                        continue;

                    sumRed += image[ni][nj].rgbtRed;
                    sumGreen += image[ni][nj].rgbtGreen;
                    sumBlue += image[ni][nj].rgbtBlue;
                    count++;
                }
            }

            // Store averaged values in copy
            copy[i][j].rgbtRed = (BYTE) round((float) sumRed / count);
            copy[i][j].rgbtGreen = (BYTE) round((float) sumGreen / count);
            copy[i][j].rgbtBlue = (BYTE) round((float) sumBlue / count);
        }
    }

    // Copy blurred results back into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    free(copy);
}

// Detect edges using Sobel operator
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // Create copy of image
    // RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE)); // manually allocate memory for 2D array
    // (*copy) creates a pointer to an array, *copy[width] would create an array of pointers!!!
    RGBTRIPLE (*copy)[width] = calloc(height, sizeof(*copy)); // let compiler allocate memory for the 2D array
    if (copy == NULL) return;

    // Loop through each pixel
    for (int i = 0; i < height; i++)
    // i = 0
    {
        for (int j = 0; j < width; j++)
        // j = 0
        {
            float gxRed = 0, gxGreen = 0, gxBlue = 0;
            float gyRed = 0, gyGreen = 0, gyBlue = 0;

            // Check each neighbor in 3x3 box
            for (int rowOffset = -1; rowOffset <= 1; rowOffset++)
            // rowOffset = -1
            {
                for (int colOffset = -1; colOffset <= 1; colOffset++)
                // colOffset = -1
                {
                    int neighborRow = i + rowOffset; // -1
                    int neighborCol = j + colOffset; // -1

                    // Skip neighbors outside the image
                    if (neighborRow < 0 || row >= height || neighborCol < 0 || neighborCol >= width)
                        continue;

                    RGBTRIPLE neighbor = image[row][col]; // image[-1][-1]

                    // Adjust index for kernel grid positions
                    int kernelRow = rowOffset + 1; // kernelRow = 0
                    int kernelCol = colOffset + 1; // kernelCol = 0

                    // Multiply kernel cell value wand the RGB value of the neighbour pixel
                    gxRed   += Gx[kernelRow][kernelCol] * neighbor.rgbtRed; // Gx[0][0] ("-1") * neighbor.rgbtRed ("255")
                    gxGreen += Gx[kernelRow][kernelCol] * neighbor.rgbtGreen;
                    gxBlue  += Gx[kernelRow][kernelCol] * neighbor.rgbtBlue;

                    gyRed   += Gy[kernelRow][kernelCol] * neighbor.rgbtRed;
                    gyGreen += Gy[kernelRow][kernelCol] * neighbor.rgbtGreen;
                    gyBlue  += Gy[kernelRow][kernelCol] * neighbor.rgbtBlue;
                }
            }

            // Compute gradient magnitude
            int finalRed   = round(sqrt(gxRed * gxRed + gyRed * gyRed)); //
            int finalGreen = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int finalBlue  = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            // Cap at 255
            copy[i][j].rgbtRed   = finalRed > 255 ? 255 : finalRed;
            copy[i][j].rgbtGreen = finalGreen > 255 ? 255 : finalGreen;
            copy[i][j].rgbtBlue  = finalBlue > 255 ? 255 : finalBlue;
        }
    }

    // Copy results back to original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    free(copy); // !!!!
}


