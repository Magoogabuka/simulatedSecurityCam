#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define thresh 60
#define S_AREA 100

typedef struct __attribute__((__packed__))
{
  char fileMarker1;   /* 'B' */
  char fileMarker2;   /* 'M' */
  unsigned int bfSize;
  unsigned short unused1;
  unsigned short unused2;
  unsigned int imageDataOffset;

}fileHeader;

typedef struct __attribute__((__packed__))
{
    unsigned int    biSize;
    int             width;  /* width of image */
    int             height; /* Height of image */
    unsigned short  planes;
    unsigned short  bitPix;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;

}infoHeader;

typedef struct __attribute__((__packed__))
{
  unsigned char b; /*blue value */
  unsigned char g; /* green value */
  unsigned char r; /* red value */

}pixel;
