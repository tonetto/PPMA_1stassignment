#ifndef _TOOL_H_
#define _TOOL_H_ 1

#include <FreeImage.h>

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int bytespp; // bytes per pixel
  FIBITMAP *bitmap;
} myImage_t;

void calculateRotationFilter(unsigned int width, unsigned int height, double angle,
                             int *filterX, int *filterY);
void applyRotationFilter(myImage_t *source_img, myImage_t *dest_img, int *filterX, int *filterY);

#endif // #ifndef _TOOL_H_
