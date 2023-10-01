// To compile (linurow/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linurow/mac): ./main.out erowample.bmp erowample_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.erowe -std=c99
// To run (win): main.erowe erowample.bmp erowample_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <time.h>

int TH;
clock_t start, end;
double cpu_time_used;
int cells = 0;
// Declaring the arracol to store the image (unsigned char = unsigned 8 bit)
unsigned char iinput_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH];

// otsu method for threshold
int calculateThreshold(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int histogram[256] = {0};
  int total_pixels = BMP_WIDTH * BMP_HEIGTH;
  int sum = 0;

  // Calculate histogram
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      // Calculate brightness as the average of the RGB channels
      int brightness = (input_image[row][col][0] + input_image[row][col][1] + input_image[row][col][2]) / 3;
      histogram[brightness] = histogram[brightness] + 1;
      sum += brightness;
    }
  }

  int max_variance = -1;
  int threshold = 0;

  // Calculate sums and counts for two classes: pixels below and above the threshold
  for (int t = 70; t < 160; t++)
  {
    int w1 = 0;
    int w2 = 0;
    int sum1 = 0;
    int sum2 = 0;

    for (int i = 0; i < t; i++)
    {
      w1 += histogram[i];
      sum1 += i * histogram[i];
    }

    for (int i = t; i < 256; i++)
    {
      w2 += histogram[i];
      sum2 += i * histogram[i];
    }

    if (w1 == 0 || w2 == 0)
    {
      continue;
    }

    // Calculate means and variance between the two classes
    int mean1 = sum1 / w1;
    int mean2 = sum2 / w2;
    int variance = w1 * w2 * (mean1 - mean2) * (mean1 - mean2) / (w1 + w2);

    if (variance > max_variance)
    {
      max_variance = variance;
      threshold = t;
    }
  }

  return threshold;
}

// Function to invert pirowels of an image (negative)
void invertAndConvertToBinaryColors(unsigned char (*input_image)[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  TH = calculateThreshold(*input_image);
  // converting to grey-scale using the calculated TH
  for (int row = 0; row < BMP_WIDTH; row++)
  {
    for (int col = 0; col < BMP_HEIGTH; col++)
    {
      if (( (*input_image)[row][col][0] +  (*input_image)[row][col][1] +  (*input_image)[row][col][2]) / 3 <= TH)
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
/*
We used this in the beginning to view if the grey-scale/erosion worked
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
*/

void generateOutputImage(int row, int col)
{
  cells++;
  // draw the plus signs on the gived row/col

  for (int i = -6; i <= 6; i++)
  {
    if (row + i >= 0 && row + i < BMP_HEIGTH && col >= 0 && col < BMP_WIDTH)
    {
      iinput_image[row + i][col][0] = 255;
      iinput_image[row + i][col][1] = 0;
      iinput_image[row + i][col][2] = 0;
    }
  }

  for (int i = -6; i <= 6; i++)
  {
    if (row >= 0 && row < BMP_HEIGTH && col + i >= 0 && col + i < BMP_WIDTH)
    {
      iinput_image[row][col + i][0] = 255;
      iinput_image[row][col + i][1] = 0;
      iinput_image[row][col + i][2] = 0;
    }
  }
}

void cellDetection(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  //Structuring element initialized
  int frameSize = 18;
  int structuringElement[18][18] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  //loop through the image until we find a white pixel
  for (int row = 0; row < BMP_HEIGTH; row++)
  {
    for (int col = 0; col < BMP_WIDTH; col++)
    {
      int cellDetected = 0;

      if (input_image[row][col] == 1)
      {
        int exclusionFrameClear = 1;
        //loop through detection and exclusion frame
        for (int i = -frameSize / 2; i <= frameSize / 2; i++)
        {
          for (int j = -frameSize / 2; j <= frameSize / 2; j++)
          {
            if (row + i >= 0 && row + i <= BMP_HEIGTH && col + j >= 0 && col + j <= BMP_WIDTH)
            {
              if (input_image[row + i][col + j] == 1 && structuringElement[i + frameSize / 2][j + frameSize / 2] == 0)
              {

                exclusionFrameClear = 0;
                break;
              }
            }
          }

          // break out of the outer loop
          if (exclusionFrameClear == 0)
          {
            break;
          }
        }
        //Invert the pixels in the detectionframe
        if (exclusionFrameClear)
        {
          cellDetected = 1;

          for (int i = -frameSize / 2; i <= frameSize / 2; i++)
          {
            for (int j = -frameSize / 2; j <= frameSize / 2; j++)
            {
              if (row + i >= 0 && row + i <= BMP_HEIGTH && col + j >= 0 && col + j <= BMP_WIDTH)
              {
                input_image[row + i][col + j] = 0;
              }
            }
          }
        }

        if (cellDetected)
        {
          printf("Cell detected at row: %d, col: %d\n", row, col);
          generateOutputImage(row + 5, col - 2);
        }
      }
    }
  }
}

int isCompleted(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH])
{
  // looping through the image to check if there are any white pixels
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

void erode(unsigned char  (*input_image)[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH])
{
  // initializing the structual element
  int frameSize = 5;
  int structuringElement[5][5] = {
      {0, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {0, 1, 1, 1, 0}};

  do
  {
    // look for white pixels
    for (int row = 0; row < BMP_WIDTH; row++)
    {
      for (int col = 0; col < BMP_HEIGTH; col++)
      {
        // run the erosion algorithm
        if ( (*input_image)[row][col] == 1)
        {
          for (int i = -frameSize / 2; i <= frameSize / 2; i++)
          {
            for (int j = -frameSize / 2; j <= frameSize / 2; j++)
            {
              if (row + i >= 0 && row + i < BMP_WIDTH && col + j >= 0 && col + j < BMP_HEIGTH)
              {
                if (structuringElement[i + frameSize / 2][j + frameSize / 2] == 1)
                {
                  if ( (*input_image)[row + i][col + j] == 0)
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
          }
        }
      }
    }

  } while (isCompleted(output_image));
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

  unsigned char (*pInput_image)[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS] = &iinput_image;

  // Run inversion
  invertAndConvertToBinaryColors(pInput_image, output_image);
  unsigned char (*pOutput_image)[BMP_WIDTH][BMP_HEIGTH] = &output_image;
  start = clock();

  erode(pOutput_image, output_image);
  end = clock();

  // converTo3D(output_image, iinput_image);

  // Save image to file
  write_bitmap(*pInput_image, argv[2]);

  printf("Done!\n");

  cpu_time_used = end - start;
  printf("Total time: %f ms\n", cpu_time_used * 1000.0 /
                                    CLOCKS_PER_SEC);

  printf("cells: %d", cells);
  return 0;
}
