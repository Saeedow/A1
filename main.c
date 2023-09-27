// To compile (linurow/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linurow/mac): ./main.out erowample.bmp erowample_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.erowe -std=c99
// To run (win): main.erowe erowample.bmp erowample_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <time.h>

const int TH = 130;
clock_t start, end;
double cpu_time_used;
// Declaring the arracol to store the image (unsigned char = unsigned 8 bit)
unsigned char iinput_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH];

// otsu method for threshold
void otsuCriteria(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int th)
{
}

// Function to invert pirowels of an image (negative)
void invertAndConvertToBinaryColors(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      if ((input_image[row][col][0] + input_image[row][col][1] + input_image[row][col][2]) / 3 <= TH)
      {
        output_image[row][col] = 0;
      }
      else
      {
        output_image[row][col] = 1;
      }
    }
  }
}

void converTo3D(unsigned char twoD[BMP_WIDTH][BMP_HEIGTH], unsigned char threeD[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {

      threeD[row][col][0] = twoD[row][col];
      threeD[row][col][1] = twoD[row][col];
      threeD[row][col][2] = twoD[row][col];
    }
  }
}

void generateOutputImage(int row, int col)
{
  // printf("the row is: %d,  the col: %d \n ", row, col);

  for (int i = 0; i <= 2; i++)
  {
    iinput_image[row + i][col][0] = 255;
    iinput_image[row + i][col][1] = 0;
    iinput_image[row + i][col][2] = 0;
    iinput_image[row - i][col][0] = 255;
    iinput_image[row - i][col][1] = 0;
    iinput_image[row - i][col][2] = 0;
    iinput_image[row][col + i][0] = 255;
    iinput_image[row][col + i][1] = 0;
    iinput_image[row][col + i][2] = 0;
    iinput_image[row][col - i][0] = 255;
    iinput_image[row][col - i][1] = 0;
    iinput_image[row][col - i][2] = 0;
  }
}

void cellDetection(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_HEIGTH; row++)
  {
    for (int col = 0; col < BMP_WIDTH; col++)
    {
      if (input_image[row][col] == 1)
      {
        int cellDetected = 0;

        for (int i = -6; i <= 5; i++)
        {
          for (int j = -6; j <= 5; j++)
          {
            if (row + i >= 0 && row + i < BMP_HEIGTH && col + j >= 0 && col + j < BMP_WIDTH)
            {

              if (input_image[row + i][col + j] == 1)
              {
                int exclusionFrameClear = 1;
                for (int x = -1; x <= 1; x++)
                {
                  for (int y = -1; y <= 1; y++)
                  {
                    if (row + i + x >= 0 && row + i + x < BMP_HEIGTH && col + i + y >= 0 && col + i + y < BMP_WIDTH &&
                        input_image[row + i + x][col + i + y] != 0)
                    {
                      exclusionFrameClear = 0;
                      break;
                    }
                  }
                }

                if (exclusionFrameClear)
                {
                  cellDetected = 1;

                  for (int x = -6; x <= 5; x++)
                  {
                    for (int y = -6; y <= 5; y++)
                    {
                      if (row + i + x >= 0 && row + i + x <= BMP_HEIGTH && col + j + y >= 0 && col + j + y <= BMP_WIDTH)
                      {
                        input_image[row + i + x][col + j + y] = 0;
                      }
                    }
                  }
                }
                exclusionFrameClear = 0;
              }
            }
          }
        }

        if (cellDetected)
        {
          generateOutputImage(row, col);
        }
      }
    }
  }
}

int isCompleted(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      if (input_image[row][col] == 1)
      {
        cellDetection(input_image);
        return 1;
      }
    }
  }
  return 0;
}

void erode(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  while (isCompleted(input_image))
  {
    for (int row = 0; row < BMP_WIDTH; row++)
    {
      for (int col = 0; col < BMP_HEIGTH; col++)
      {
        if (input_image[row][col] == 1)
        {
          for (int i = -1; i <= 1; i++)
          {
            for (int j = -1; j <= 1; j++)
            {
              if (row + i >= 0 && row + i <= BMP_HEIGTH && col + j >= 0 && col + j <= BMP_WIDTH && (input_image[row + i][col - j] == 0 || input_image[row][col + j] == 0))
              {
                input_image[row][col] = 0;
              }
            }
          }
        }
      }
    }
  }
}

// Main function
int main(int argc, char **argv)
{
  // argc counts how macol arguments are passed
  // argv[0] is a string with the name of the program
  // argv[1] is the first command line argument (input image)
  // argv[2] is the second command line argument (output image)

  // Checking that 2 arguments are passed
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Example program - 02132 - A1\n");
  // Load image from file
  read_bitmap(argv[1], iinput_image);

  // Run inversion
  invertAndConvertToBinaryColors(iinput_image, output_image);

  start = clock();

  erode(output_image);
  end = clock();

  // converTo3D(output_image, iinput_image);

  // Save image to file
  write_bitmap(iinput_image, argv[2]);

  printf("Done!\n");

  cpu_time_used = end - start;
  printf("Total time: %f ms\n", cpu_time_used * 1000.0 /
                                    CLOCKS_PER_SEC);
  return 0;
}
