#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

class ImageBMP
{
private:
  int width;
  int height;
  int totalSize;
  int bitDepth;
  unsigned char header[54];
  unsigned char colorTable[1024];
  unsigned char *dataImage;
  unsigned char *dataImageOut;

public:
  ImageBMP(const char *linkFile)
  {
    // Tao doi tuong de doc file
    FILE *fileIn;
    // https://vietjack.com/thu-vien-c/ham-fopen-trong-c.jsp
    // Doc file bang ham fopen
    fileIn = fopen(linkFile, "rb");

    int i;
    // Doc Image header
    for (i = 0; i < 54; i++)
    {
      // Ham getc lay ky tu ke tiep (1 unsigned char) tu file da cho, va position indicator (chi bao vi tri) tu tang
      // https://vietjack.com/thu-vien-c/ham-getc-trong-c.jsp
      header[i] = getc(fileIn);
    }
    width = *(int *)&header[18];    // offset 18 luu thong tin chieu rong
    height = *(int *)&header[22];   // offset 22 luu thong tin chieu cao
    bitDepth = *(int *)&header[28]; // offset 28 luu thong tin so bit tren moi pixel anh
    totalSize = width * height;

    // Kiem tra bitDepth de doc color table
    if (bitDepth <= 8)
    {
      // https://vietjack.com/thu-vien-c/ham-fread-trong-c.jsp
      fread(colorTable, sizeof(unsigned char), 1024, fileIn);
      dataImage = (unsigned char *)calloc(totalSize, sizeof(unsigned char));
      dataImageOut = (unsigned char *)calloc(totalSize, sizeof(unsigned char));
      fread(dataImage, sizeof(unsigned char), (height * width), fileIn);
    }
    else
    {
      //RGB normal
      dataImage = (unsigned char *)calloc(totalSize, 3 * sizeof(unsigned char));
      dataImageOut = (unsigned char *)calloc(totalSize, 3 * sizeof(unsigned char));
      fread(dataImage, sizeof(unsigned char), totalSize * 3, fileIn);
    }

    fclose(fileIn);
  };

  void copyToNewImage(const char *newfile)
  {
    FILE *fileOut = fopen(newfile, "wb");
    // Ghi thong tin header ra file moi
    fwrite(this->header, sizeof(unsigned char), 54, fileOut);
    // ghi thong tin mau
    if (bitDepth <= 8)
    {
      fwrite(this->colorTable, sizeof(unsigned char), 1024, fileOut);
      fwrite(this->dataImage, sizeof(unsigned char), (height * width), fileOut);
    }
    else
    {
      fwrite(this->dataImage, sizeof(unsigned char), this->totalSize * 3, fileOut);
    }
    fclose(fileOut);
  }

  void rotate180(const char *newfile)
  {
    FILE *fileOut = fopen(newfile, "wb");
    // Ghi thong tin header ra file moi
    fwrite(this->header, sizeof(unsigned char), 54, fileOut);
    // ghi thong tin mau
    if (bitDepth <= 8)
    {
      fwrite(this->colorTable, sizeof(unsigned char), 1024, fileOut);
      // Ghi thong tin hinh anh
      int i, j;
      for (i = 0; i < width; i++)
      {
        for (j = 0; j < height; j++)
        {
        }
      }
      fwrite(dataImageOut, sizeof(unsigned char), (height * width), fileOut);
    }
    else
    {
      for (int i = 0; i < totalSize; i++)
      {
        dataImageOut[i * 3] = dataImage[(totalSize - 1 - i) * 3];
        dataImageOut[i * 3 + 1] = dataImage[(totalSize - 1 - i) * 3 + 1];
        dataImageOut[i * 3 + 2] = dataImage[(totalSize - 1 - i) * 3 + 2];
      }
      fwrite(this->dataImageOut, sizeof(unsigned char), this->totalSize * 3, fileOut);
    }
    fclose(fileOut);
  }
  ~ImageBMP()
  {
    free(this->dataImage);
    free(this->dataImageOut);
  };
};

int main()
{
  ImageBMP image("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\IG.bmp");
  // image.copyToNewImage("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\IG_copy.bmp");
  image.rotate180("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\IG_180.bmp");
}
