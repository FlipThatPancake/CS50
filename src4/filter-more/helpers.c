#include "helpers.h"
#include <math.h>
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
    // Sobel kernels
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

    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (copy == NULL)
    {
        return;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gxRed = 0, gxGreen = 0, gxBlue = 0;
            float gyRed = 0, gyGreen = 0, gyBlue = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                        continue;

                    int kernelX = Gx[di + 1][dj + 1];
                    int kernelY = Gy[di + 1][dj + 1];

                    RGBTRIPLE pixel = image[ni][nj];

                    gxRed += kernelX * pixel.rgbtRed;
                    gxGreen += kernelX * pixel.rgbtGreen;
                    gxBlue += kernelX * pixel.rgbtBlue;

                    gyRed += kernelY * pixel.rgbtRed;
                    gyGreen += kernelY * pixel.rgbtGreen;
                    gyBlue += kernelY * pixel.rgbtBlue;
                }
            }

            // Compute final gradient magnitude
            int finalRed = (int) round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int finalGreen = (int) round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int finalBlue = (int) round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            // Cap values at 255
            copy[i][j].rgbtRed = finalRed > 255 ? 255 : finalRed;
            copy[i][j].rgbtGreen = finalGreen > 255 ? 255 : finalGreen;
            copy[i][j].rgbtBlue = finalBlue > 255 ? 255 : finalBlue;
        }
    }

    // Copy edge-detected results back into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    free(copy);
}
