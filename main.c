// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

const int TH = 120;

// Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      output_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
    }
  }
}

void convertToBinary(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (output_image[x][y] <= TH)
      {
        output_image[x][y] = 0;
      }
      else
      {
        output_image[x][y] = 255;
      }
    }
  }
}
void converTo3D(unsigned char twoD[BMP_WIDTH][BMP_HEIGTH], unsigned char threeD[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {

      threeD[x][y][0] = twoD[x][y];
      threeD[x][y][1] = twoD[x][y];
      threeD[x][y][2] = twoD[x][y];
    }
  }
}
int isCompleted(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (input_image[x][y] == 255)
      {
        return 1;
      }
    }
  }
  return 0;
}
void erode(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{

  int counter = 1;
  do
  {
    for (int x = 1; x < BMP_WIDTH - 1; x++)
    {
      for (int y = 1; y < BMP_HEIGTH - 1; y++)
      {

        if (input_image[x + 1][y] == 255 && input_image[x - 1][y] == 255 && input_image[x][y + 1] == 255 && input_image[x][y - 1] == 255 && input_image[x][y - 1] == 255)
        {
          input_image[x][y] = 0;
        }
      }
    }

    counter++;

  } while (isCompleted(input_image));

  printf("%d", counter);
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH];

// Main function
int main(int argc, char **argv)
{
  // argc counts how may arguments are passed
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
  read_bitmap(argv[1], input_image);

  // Run inversion
  invert(input_image, output_image);

  convertToBinary(output_image);

  erode(output_image);

  converTo3D(output_image, input_image);
  // Save image to file
  write_bitmap(input_image, argv[2]);

  printf("Done!\n");
  return 0;
}
