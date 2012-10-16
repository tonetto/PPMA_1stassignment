/***************************************
 * Puzzle work 1b. Homework
 * Description: Simple C code using FreeImage Library to load a bitmap image
 *              that will be later on transformed, and this code should be
 *              parallelized using MPI. It assumes the user is using a simple
 *              24-bit bitmap image RGB encoded. A sample debian.bmp is
 *              provided and is encouraged to be used.
 *
 * Hint: to compile, just run make at the base directory
 *
 ***************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tool.h"

#define _IMAGE_ "debian.bmp"

int main(int argc, char *argv[])
{
  // sanity check
  if (argc == 1) {
    printf("You should pass " _IMAGE_ " as a parameter to this program\n");
    return 1;
  }
  myImage_t myBitmap;
  myImage_t resultBMP;
  // starting the FreeImage Library
  FreeImage_Initialise(FALSE);

  myBitmap.bitmap = FreeImage_Load(FIF_BMP, argv[1], BMP_DEFAULT);
  if (myBitmap.bitmap) { // bitmap successfully loaded!
    myBitmap.width    = FreeImage_GetWidth(myBitmap.bitmap);
    myBitmap.height   = FreeImage_GetHeight(myBitmap.bitmap);
    myBitmap.bytespp  = FreeImage_GetLine(myBitmap.bitmap) / myBitmap.width;

    resultBMP.width   = myBitmap.width;
    resultBMP.height  = myBitmap.height;
    resultBMP.bytespp = myBitmap.bytespp;
    resultBMP.bitmap  = FreeImage_Allocate(myBitmap.width, myBitmap.height, (myBitmap.bytespp*8),
                                           FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

    if (!resultBMP.bitmap) {
      goto out_err;
    }

    // The actual computation should go here
    int *filterX = (int*)malloc(myBitmap.width * myBitmap.height * sizeof(int));
    int *filterY = (int*)malloc(myBitmap.width * myBitmap.height * sizeof(int));
    if (filterX == NULL) {
      printf("Couldn't allocate memory. Exiting...\n");
      goto out_err;
    }
    if (filterY == NULL) {
      printf("Couldn't allocate memory. Exiting...\n");
      free(filterY);
      goto out_err;
    }

    calculateRotationFilter(myBitmap.width, myBitmap.height, 0.23, filterX, filterY);
    applyRotationFilter(&myBitmap, &resultBMP, filterX, filterY);

    // save the results
    if (FreeImage_Save(FIF_BMP, resultBMP.bitmap, "edited.bmp", 0)) {
      printf("\nTransformed image saved.\n");
    } else {
      printf("\nProblem while trying to save the image.\n");
    }
    free(filterX);
    free(filterY);
  } else {
    printf("\nCouldn't load the image. Please make sure you're using " _IMAGE_ "\n");
    printf("Or at least a 24-bit bitmap RGB encoded image.\n");
    goto out_err;
  }

 out_err:
  if (resultBMP.bitmap) {
    // Needs to be called to avoid memory leak
    FreeImage_Unload(resultBMP.bitmap);
  }
  if (myBitmap.bitmap) {
    // Needs to be called to avoid memory leak
    FreeImage_Unload(myBitmap.bitmap);
  }
  // Finalizing the FreeImage Library
  FreeImage_DeInitialise();
  return 0;
}


// This is the actual function that can be parallelized
void calculateRotationFilter(unsigned int width, unsigned int height, double angle,
                             int *filterX, int *filterY)
{
  int x = 0;
  int y = 0;
  for (y=0; y < height; y++) {
    for (x=0; x < width; x++) {
      // calculating X'
      filterX[x + (y*width)] = (int)round((x*cos(angle)) - (y*sin(angle)));
      // calculating Y'
      filterY[x + (y*width)] = (int)round((x*sin(angle)) + (y*cos(angle)));
    }
  }
}

void applyRotationFilter(myImage_t *source_img, myImage_t *dest_img, int *filterX, int *filterY)
{
  int x = 0;
  int y = 0;

  BYTE *source_bits = (BYTE*)FreeImage_GetBits(source_img->bitmap);
  BYTE *dest_bits   = (BYTE*)FreeImage_GetBits(dest_img->bitmap);

  for (y=0; y < source_img->height; y++) {
    for (x=0; x <source_img->width; x++) {
      int _x = filterX[x + (y*source_img->width)];
      int _y = filterY[x + (y*source_img->width)];

      if ((_x < 0) || (_x >= source_img->width) ||
          (_y < 0) || (_y >= source_img->height)) {
        continue;
      }

      dest_bits[(_x * dest_img->bytespp) + (_y * (dest_img->bytespp * dest_img->width)) + FI_RGBA_RED] =
        source_bits[(x * dest_img->bytespp) + (y * (dest_img->bytespp * dest_img->width)) + FI_RGBA_RED];
      dest_bits[(_x * dest_img->bytespp) + (_y * (dest_img->bytespp * dest_img->width)) + FI_RGBA_GREEN] =
        source_bits[(x * dest_img->bytespp) + (y * (dest_img->bytespp * dest_img->width)) + FI_RGBA_GREEN];
      dest_bits[(_x * dest_img->bytespp) + (_y * (dest_img->bytespp * dest_img->width)) + FI_RGBA_BLUE] =
        source_bits[(x * dest_img->bytespp) + (y * (dest_img->bytespp * dest_img->width)) + FI_RGBA_BLUE];
    }
  }
}
