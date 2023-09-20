// To compile (linurow/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linurow/mac): ./main.out erowample.bmp erowample_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.erowe -std=c99
// To run (win): main.erowe erowample.bmp erowample_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

const int TH = 120;
// Declaring the arracol to store the image (unsigned char = unsigned 8 bit)
unsigned char iinput_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH];

// Function to invert pirowels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      output_image[row][col] = (input_image[row][col][0] + input_image[row][col][1] + input_image[row][col][2]) / 3;
    }
  }
}

void convertToBinarcol(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      if (output_image[row][col] <= TH)
      {
        output_image[row][col] = 0;
      }
      else
      {
        output_image[row][col] = 255;
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

int isCompleted(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      if (input_image[row][col] == 255)
      {
        return 1;
      }
    }
  }
  return 0;
}

void generateOutputImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int row, int col)
{

  for (int i = 0; i < 100; i++)
  {
    input_image[row + i][col][0] = 255;
    input_image[row + i][col][1] = 0;
    input_image[row + i][col][2] = 0;
    input_image[row - i][col][0] = 255;
    input_image[row - i][col][1] = 0;
    input_image[row - i][col][2] = 0;
    input_image[row][col + i][0] = 255;
    input_image[row][col + i][1] = 0;
    input_image[row][col + i][2] = 0;
    input_image[row][col - i][0] = 255;
    input_image[row][col - i][1] = 0;
    input_image[row][col - i][2] = 0;
  }
}

void cellDetection(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      if (input_image[row][col] == 255)
      {
        if (row + 6 < BMP_HEIGTH && row - 6 <= 0 && row + 6 < BMP_HEIGTH && row - 6 <= 0 && col + 6 < BMP_HEIGTH && col - 6 <= 0 && col + 6 < BMP_HEIGTH && col - 6 <= 0)
        {
        }
      }
    }
  }
}

void erode(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  int counter = 0;
  while (isCompleted(input_image))
  {
    for (int row = 0; row < BMP_WIDTH; row++)
    {
      for (int col = 0; col < BMP_HEIGTH; col++)
      {
        // top left
        if (input_image[row][col] == 255)
        {
          if (row == 0 && col == 0)
          {
            if (input_image[row + 1][col] == 0 || input_image[row][col + 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }
          // top right
          if (row == 0 && col == BMP_WIDTH)
          {
            if (input_image[row + 1][col] == 0 || input_image[row][col - 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }
          // bottom left
          if (row == BMP_HEIGTH && col == 0)
          {
            if (input_image[row - 1][col] == 0 || input_image[row][col + 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }
          // bottom right
          if (row == BMP_HEIGTH && col == BMP_WIDTH)
          {
            if (input_image[row - 1][col] == 0 || input_image[row][col - 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }

          // first row
          if (row == 0 && col > 0 && col < BMP_WIDTH)
          {
            if (input_image[row + 1][col] == 0 || input_image[row][col - 1] == 0 || input_image[row][col + 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }

          // last row

          if (row == BMP_HEIGTH && col > 0 && col < BMP_WIDTH)
          {
            if (input_image[row - 1][col] == 0 || input_image[row][col - 1] == 0 || input_image[row][col + 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }

          // first column
          if (col == 0 && row > 0 && row < BMP_HEIGTH)
          {
            if (input_image[row - 1][col] == 0 || input_image[row][col + 1] == 0 || input_image[row + 1][col] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }
          // last column
          if (col == BMP_HEIGTH && row > 0 && row < BMP_HEIGTH)
          {
            if (input_image[row - 1][col] == 0 || input_image[row + 1][col] == 0 || input_image[row][col - 1] == 0)
            {
              input_image[row][col] = 0;
              continue;
            }
          }

          if (input_image[row + 1][col] == 0 || input_image[row - 1][col] == 0 || input_image[row][col + 1] == 0 || input_image[row][col - 1] == 0)
          {
            input_image[row][col] = 0;
            continue;
          }
        }
      }
    }

    counter++;
  }

  printf("%d", counter);
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
  invert(iinput_image, output_image);

  convertToBinarcol(output_image);

  erode(output_image);

  // converTo3D(output_image, iinput_image);

  // Save image to file
  write_bitmap(iinput_image, argv[2]);

  printf("Done!\n");
  return 0;
}
