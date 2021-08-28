
#include "utils.h"

pixel **pixelArray(FILE* image, int height, int width);

int main(int argc, char *argv[])
{
  FILE *image1, *image2, *image3;
  fileHeader data, data2;
  infoHeader info, info2;
  pixel **pixelArray(FILE* image, int height, int width), **subPixelData, **pixelData, **pixelData2;
  int i, j, counter;
  unsigned char imageLuminance, backgLuminance, difference;
  counter = 0;

/*-------------------------------------------------------------------------
First if statement checks that the number of arguements entered is enough
--------------------------------------------------------------------------*/

  if(argc != 4)
  {
    fprintf(stderr, "ERROR: NOT ENOUGH ARGUEMENTS\n");

    return -1;
  }

  image1 = fopen(argv[1], "r");
  image2 = fopen(argv[2], "r");


  if((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "ERROR: NO IMAGE FILE DETECTED\n");
    return -1;
  }

  if(fread(&data, sizeof(data), 1, image1) != 1 )
  {
    fprintf(stderr, "ERROR READING IN FILEHEADER FOR IMAGE1.\n");
    return -1;
  }

  if (fread(&data2, sizeof(data2), 1, image2) != 1 )
  {
    fprintf(stderr, "ERROR READING IN FILEHEADER FOR IMAGE2.\n");
    return -1;
  }

  /*--------------------------------------------------------------------------------------
                  USED FOR CHECKING THE OUTPUT OF THE STRUCTURES
    printf("FILEMARKER1\n IMAGE1: %c | IMAGE2: %c\n", data.fileMarker1, data2.fileMarker1);
    printf("FILEMARKER2\n IMAGE1: %c | IMAGE2: %c\n", data.fileMarker2, data2.fileMarker2);
----------------------------------------------------------------------------------------------*/

  /*--------------------------------------------------------
  Checking to make sure that the images are of type BMP
  and then terminating with an EXIT_FAILURE return
  ---------------------------------------------------------*/

  if(((data.fileMarker1 != 'B') && (data.fileMarker2 != 'M')) || ((data2.fileMarker1 != 'B') && (data2.fileMarker2 != 'M')))
  {
    fclose(image1);
    fprintf(stderr, "ERROR: FILE FORMAT IS INCORRECT.\n");

    return -1;
  }

  /*---------------------------------------------------------
                     PIXELDATA ARRAYS
  Malloc dynamincally creates a memory allocation array and
  allows the program to access the memory through use of pointers.
  Then we use for loops to allocate positions in the arrays
  pixelData for image1 and pixelData2 for image2.
  ---------------------------------------------------------- */

  if(fread(&info, sizeof(info), 1, image1) != 1 )
  {
    fprintf(stderr, "ERROR READING IN FILEHEADER FOR IMAGE1.\n");
    return -1;
  }

  if(fread(&info2, sizeof(info2), 1, image2) != 1 )
  {
    fprintf(stderr, "ERROR READING IN FILEHEADER FOR IMAGE2.\n");
    return -1;
  }

  /*---------------------------------------------------------------------
          USED FOR CHECKING THAT INPUT TO STRUCTURES OCCURED
  printf("HEIGHT\nIMAGE1: %d\nIMAGE2: %d\n", info.height, info2.height);
  printf("WIDTH\nIMAGE1: %d\nIMAGE2: %d\n", info.width, info2.width);
  ----------------------------------------------------------------------*/

  if((info.height != info2.height) || (info.width != info2.width))
  {
    fprintf(stderr, "IMAGE DIMENSIONS DO NOT MATCH.\n");
    return -1;
  }

  pixelData = pixelArray(image1, info.height, info.width);
  pixelData2 = pixelArray(image2, info2.height, info2.width);

  if((fclose(image1) != 0) || (fclose(image2) != 0))
  {
    fprintf(stderr, "ERROR CLOSING IMAGE. APPLICATION TERMINATED\n");
    return -1;
  }

  /*
  printf("\npixelData = r:%d g:%d b:%d\n", pixelData[1][1].r, pixelData[1][1].g, pixelData[1][1].b);
  printf("pixelData2 = r:%d g:%d b:%d\n", pixelData2[1][1].r, pixelData2[1][1].g, pixelData2[1][1].b);
  */


    /*---------------------------------------------------------
                          LUMINENCE
    Again using Malloc for dynamic allocation. then using a for
    loop to initialize the array for the sizeof pixel structure.
    ----------------------------------------------------------*/

  subPixelData = (pixel**)malloc(info.height*sizeof(pixel*));
  for(i = 0; i<info.height; i++)
  {
    subPixelData[i] = (pixel*)malloc(info.width*sizeof(pixel));
  }
  if(subPixelData == 0)
  {
    fprintf(stderr, "ERROR: ALLOCATION HAS FAILED.\n");

    return -1;
  }

   /*----------------------
   LUMINENCE SUBTRACTION
   -----------------------*/

  for(i=0; i<info.height; i++)
  {
    for(j=0; j<info.width; j++)
    {
      imageLuminance = (pixelData[i][j].r + pixelData[i][j].g + pixelData[i][j].b)/3;
      backgLuminance = (pixelData2[i][j].r + pixelData2[i][j].g + pixelData2[i][j].b)/3;
      difference = abs(backgLuminance - imageLuminance);

      if(difference > thresh)
      {
        difference = 250;
        counter++;
      }
      else
      {
        difference = 0;
      }

      subPixelData[i][j].r = difference;
      subPixelData[i][j].g = difference;
      subPixelData[i][j].b = difference;

      }
    }


  /*
  printf("\ndifference = %d\n", difference);
  printf("subPixelData = r:%d g:%d b:%d\n", subPixelData[250][250].r, subPixelData[250][250].g, subPixelData[250][250].b);
  */

  image3 = fopen(argv[3], "w");

  if (image3 == NULL)
  {
    fprintf(stderr, "ERROR: FILE CANNOT BE OPENED OR CREATED.\n");

    return-1;
  }

  if((fwrite(&data, sizeof(fileHeader), 1, image3) != 1) || (fwrite(&info, sizeof(infoHeader), 1, image3)) != 1)
  {
    fprintf(stderr, "ERROR WRITING FILE INTO STRUCT FILEHEADER.\n");
  }

  for(i = 0; i<info.height; i++)
  {
      if(fwrite(subPixelData[i], sizeof(pixel), info.width, image3) != info.width)
      {
        fprintf(stderr, "ERROR READING INTO ARRAY FOR IMAGE3.\n");
        return -1;
      }
      for(j = 0; j < info.width%4; j++)
      {
        fputc('\0', image3); /*fpuc to write a single character into the file for each loop, incrementingly, \0 writes padding at end of line */
      }
  }


  if(fclose(image3) != 0)
  {
    fprintf(stderr, "ERROR CLOSING IMAGE 3 PROGRAM TERMINATED.\n");
    return -1;
  }
  /*---------------------------------------------------
  Checks for the intrusion in the image depending on
  the image S_AREA.
  -----------------------------------------------------*/

  if(counter == 0)
  {
    printf("No intruding object has been found (counter = %d)\n", counter);
  }
  else if(counter <S_AREA)
  {
    printf("ALARM: A small intruding object has been detected (counter = %d)\n", counter);
  }
  else if(counter >= S_AREA)
  {
    printf("ALARM: A large intruding object has been detected (counter = %d)\n", counter);
  }

  /*
   freeing the data stored in the pixeldata arrays using malloc
  */

  for(i=0; i<info.height; i++ )
  {
    free(subPixelData[i]);
  }

  free(pixelData);
  free(pixelData2);
  free(subPixelData);


  return 0;
}


/* FUNCTION TO ASSIGN DATA TO PIXELDATA ARRAY */

pixel **pixelArray(FILE* image, int height, int width)
{
  pixel **pixelData;
  int i, j;

  pixelData = (pixel**)malloc(height*sizeof(pixel*));

  for(i = 0; i<height; i++)
  {
    pixelData[i] = (pixel*)malloc(width*sizeof(pixel));
  }

  if(pixelData == NULL)
  {
    fprintf(stderr, "ERROR: MEMORY ALLOCATION HAS FAILED.\n");

    return NULL;
  }

for(i=0; i<height; i++)
{
    if(fread(pixelData[i], 3, width, image) != width)
    {
      fprintf(stderr, "ERROR READING IN PIXELDATA ARRAY.\n");
    }
    for(j=0; j<width%4; j++)
    {
      fgetc(image);
    }
}

return pixelData;
}
