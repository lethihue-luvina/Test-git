#include <stdio.h>

int main()
{
  // Tao doi tuong de doc file
  FILE *fileIn;
  // https://vietjack.com/thu-vien-c/ham-fopen-trong-c.jsp
  // Doc file bang ham fopen
  fileIn = fopen("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\lena512.bmp", "rb");

  unsigned char header[54]; // Image header co kich thuoc la 54 byte
  // Neu so luong bit tren moi pixel <= 8 thi can co colorTable
  unsigned char colorTable[1024];

  int i, j;
  // Doc Image header
  for (i = 0; i < 54; i++)
  {
    // Ham getc lay ky tu ke tiep (1 unsigned char) tu file da cho, va position indicator (chi bao vi tri) tu tang
    // https://vietjack.com/thu-vien-c/ham-getc-trong-c.jsp
    header[i] = getc(fileIn);
  }

  int *width = (int *)&header[18];    // offset 18 luu thong tin chieu rong
  int *height = (int *)&header[22];   // offset 22 luu thong tin chieu cao
  int bitDepth = *(int *)&header[28]; // offset 28 luu thong tin so bit tren moi pixel anh

  // Kiem tra bitDepth de doc color table
  if (bitDepth <= 8)
  {
    // https://vietjack.com/thu-vien-c/ham-fread-trong-c.jsp
    fread(colorTable, sizeof(unsigned char), 1024, fileIn);
  }

  // Bien de luu tru thong tin cac pixel cua hinh anh
  unsigned char imageDataIn[*width][*height];

  // unsigned char imageDataOut[height * width];
  fread(imageDataIn, sizeof(unsigned char), (*width * *height), fileIn);

  FILE *fileOut = fopen("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\rotateleft.bmp", "wb");

  int temp = *width;
  *width = *height;
  *height = temp;

  int newwidth = *height;
  int newheight = *width;

  unsigned char imageDataOut[newwidth][newheight];
  // Ghi thong tin header ra file moi
  fwrite(header, sizeof(unsigned char), 54, fileOut);
  // ghi thong tin mau
  if (bitDepth <= 8)
  {
    fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
  }

  // Ghi thong tin hinh anh
  for (i = 0; i < newwidth; i++)
  {
    for (j = 0; j < newheight; j++)
    {
      // imageDataOut[*height - 1 - i][j] = imageDataIn[j][i];
      imageDataOut[i][j] = imageDataIn[j][newwidth - 1 - i];
    }
  }

  fwrite(imageDataOut, sizeof(unsigned char), (newwidth * newheight), fileOut);

  fclose(fileIn);
  fclose(fileOut);
}
