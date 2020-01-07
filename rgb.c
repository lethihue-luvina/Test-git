#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 255

int main()
{

  FILE *fIn = fopen("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\IG.bmp", "rb");        //Input File name
  FILE *fOut = fopen("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\IG_sepia.bmp", "wb"); //Output File name

  int i, r, g, b;
  unsigned char header[54];

  for (i = 0; i < 54; i++) //read the 54 header header from fIn
  {
    header[i] = getc(fIn);
  }

  fwrite(header, sizeof(unsigned char), 54, fOut); //write the header back

  // extract image height, width and bitDepth from imageHeader
  int width = *(int *)&header[18];
  int height = *(int *)&header[22];
  int bitDepth = *(int *)&header[28];

  int size = height * width; //calculate image size

  // Neu mang lon hon 1kb thi nen dung cap phat dong
  unsigned char *buffer;
  buffer = (unsigned char *)calloc(size, 3 * sizeof(unsigned char)); //to store the image data
  // fread(buffer, sizeof(unsigned char), size * 3, fIn);
  // fwrite(buffer, sizeof(unsigned char), size * 3, fOut);
  for (i = 0; i < size; i++)
  {
    r = 0;
    g = 0;
    b = 0;
    buffer[i * 3 + 2] = getc(fIn); //blue
    buffer[i * 3 + 1] = getc(fIn); //green
    buffer[i * 3] = getc(fIn);     //red

    //conversion formula of rgb to sepia
    r = (buffer[i * 3] * 0.393) + (buffer[i * 3 + 1] * 0.769) + (buffer[i * 3 + 2] * 0.189);
    g = (buffer[i * 3] * 0.349) + (buffer[i * 3 + 1] * 0.686) + (buffer[i * 3 + 2] * 0.168);
    b = (buffer[i * 3] * 0.272) + (buffer[i * 3 + 1] * 0.534) + (buffer[i * 3 + 2] * 0.131);

    if (r > MAX_VALUE)
    { //if value exceeds
      r = MAX_VALUE;
    }
    if (g > MAX_VALUE)
    {
      g = MAX_VALUE;
    }
    if (b > MAX_VALUE)
    {
      b = MAX_VALUE;
    }
    putc(b, fOut);
    putc(g, fOut);
    putc(r, fOut);
  }
  free(buffer);

  fclose(fOut);
  fclose(fIn);
  return 0;
}