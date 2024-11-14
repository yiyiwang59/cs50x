#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i <= height; i++)
    {
        for (int j=0; j <= width; j++)
        {
            float blue = image[i][j].rgbtBlue;
            float green = image[i][j].rgbtGreen;
            float red = image[i][j].rgbtRed;

            int avgRGB = round((blue + green + red)/3);

            image[i][j].rgbtBlue = avgRGB;
            image[i][j].rgbtGreen = avgRGB;
            image[i][j].rgbtRed = avgRGB;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i < height; i++)
    {
        RGBTRIPLE current_row[1][1];
        int j = 0;
        int k = width-1;
        for (; j < k; j++, k--)
        {
            current_row[0][0].rgbtBlue = image[i][j].rgbtBlue;
            current_row[0][0].rgbtGreen = image[i][j].rgbtGreen;
            current_row[0][0].rgbtRed = image[i][j].rgbtRed;
            image[i][j].rgbtBlue = image[i][k].rgbtBlue;
            image[i][j].rgbtGreen = image[i][k].rgbtGreen;
            image[i][j].rgbtRed = image[i][k].rgbtRed;
            image[i][k].rgbtBlue = current_row[0][0].rgbtBlue;
            image[i][k].rgbtGreen = current_row[0][0].rgbtGreen;
            image[i][k].rgbtRed = current_row[0][0].rgbtRed;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int x=0; x < height; x++)
    {
        for (int y=0; y < width; y++)
        {
            temp[x][y] = image[x][y];
        }
    }
    for (int x=0; x < height; x++)
    {
        for (int y=0; y < width; y++)
        {
            float blue_sum = 0;
            float green_sum = 0;
            float red_sum = 0;
            int total_count = 0;
            //Loop through adjacent cells
            for (int i=x-1; i<=x+1; i++)
            {
                for (int j=y-1; j<=y+1; j++)
                {
                    //check if cell exists
                    if (i < 0  || i >= height)
                    {
                        continue;
                    }
                    if (j < 0 || j >= width)
                    {
                        continue;
                    }
                    red_sum += temp[i][j].rgbtRed;
                    green_sum += temp[i][j].rgbtGreen;
                    blue_sum += temp[i][j].rgbtBlue;
                    total_count++;
                }
            }
            image[x][y].rgbtRed = round(red_sum/total_count);
            image[x][y].rgbtGreen = round(green_sum/total_count);
            image[x][y].rgbtBlue = round(blue_sum/total_count);
        }
    }
    return;
}



// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int x=0; x < height; x++)
    {
        for (int y=0; y < width; y++)
        {
            temp[x][y] = image[x][y];
        }
    }
    int gx_values[3][3] = {{-1, 0, 1},{ -2, 0, 2},{ -1, 0, 1}};
    int gy_values[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int x=0; x < height; x++)
    {
        for (int y=0; y < width; y++)
        {
            float blue_gx_sum = 0;
            float blue_gy_sum = 0;
            float green_gx_sum = 0;
            float green_gy_sum = 0;
            float red_gx_sum = 0;
            float red_gy_sum = 0;
            //Loop through adjacent cells
            int index_x = 0;
            for (int i=x-1; i<=x+1; i++)
            {
                int index_y = 0;
                for (int j=y-1; j<=y+1; j++)
                {
                    if (i >= 0 && j >= 0 && i < height && j < width)
                    {
                        red_gx_sum += (temp[i][j].rgbtRed * gx_values[index_x][index_y]);
                        red_gy_sum += (temp[i][j].rgbtRed * gy_values[index_x][index_y]);
                        green_gx_sum += (temp[i][j].rgbtGreen * gx_values[index_x][index_y]);
                        green_gy_sum += (temp[i][j].rgbtGreen * gy_values[index_x][index_y]);
                        blue_gx_sum += (temp[i][j].rgbtBlue * gx_values[index_x][index_y]);
                        blue_gy_sum += (temp[i][j].rgbtBlue * gy_values[index_x][index_y]);
                    }
                    index_y++;
                }
                index_x++;
            }
            int blue_value = round(sqrt(blue_gx_sum *  blue_gx_sum + blue_gy_sum * blue_gy_sum));
            int green_value = round(sqrt(green_gx_sum *  green_gx_sum + green_gy_sum * green_gy_sum));
            int red_value = round(sqrt(red_gx_sum *  red_gx_sum + red_gy_sum * red_gy_sum));
            // printf("sums:\nblue: %i, %i, green: %i, %i, red %i, %i\n", blue_gx_sum, blue_gy_sum, green_gx_sum, green_gy_sum, red_gx_sum, red_gy_sum);
            if (blue_value > 255)
            {
                blue_value = 255;
            }
            if (green_value > 255)
            {
                green_value = 255;
            }
            if (red_value > 255)
            {
                red_value = 255;
            }
            // printf("final\nblue: %i, green: %i, red %i\n", blue_value, green_value, red_value);
            image[x][y].rgbtRed = red_value;
            image[x][y].rgbtGreen = green_value;
            image[x][y].rgbtBlue = blue_value;

        }
    }
    return;
}
