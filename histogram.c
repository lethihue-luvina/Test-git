#include <stdio.h>
#include <math.h>

void histogramEqualisation(FILE *file, unsigned char input[], int width, int height)
{
	int hist[256] = {0};
	int newgraylevel[256] = {0};
	int i, j;
	unsigned char *p;

	// Tinh toan tan so
	for (i = 0; i < width; i++)
	{
		p = &input[i * height];
		for (j = 0; j < height; j++)
		{
			hist[(int)p[j]]++;
		}
	}
	int total = width * height;
	int curr = 0;
	// Histogram equalization
	for (i = 0; i < 256; i++)
	{
		curr += hist[i];
		newgraylevel[i] = round(((float)curr) * 255 / total);
	}
	for (i = 0; i < width; i++)
	{
		p = &input[i * height];
		for (j = 0; j < height; j++)
			p[j] = (unsigned char)newgraylevel[p[j]];
		fwrite(p, sizeof(unsigned char), height, file);
	}
}

int main()
{
	// Tao doi tuong de doc file
	FILE *fileIn;
	// https://vietjack.com/thu-vien-c/ham-fopen-trong-c.jsp
	// Doc file bang ham fopen
	fileIn = fopen("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\boats.bmp", "rb");

	unsigned char header[54]; // Image header co kich thuoc la 54 byte
	// Neu so luong bit tren moi pixel <= 8 thi can co colorTable
	unsigned char colorTable[1024];

	int i;
	// Doc Image header
	for (i = 0; i < 54; i++)
	{
		// Ham getc lay ky tu ke tiep (1 unsigned char) tu file da cho, va position indicator (chi bao vi tri) tu tang
		// https://vietjack.com/thu-vien-c/ham-getc-trong-c.jsp
		header[i] = getc(fileIn);
	}

	int width = *(int *)&header[18];		// offset 18 luu thong tin chieu rong
	int height = *(int *)&header[22];		// offset 22 luu thong tin chieu cao
	int bitDepth = *(int *)&header[28]; // offset 28 luu thong tin so bit tren moi pixel anh

	// Kiem tra bitDepth de doc color table
	if (bitDepth <= 8)
	{
		// https://vietjack.com/thu-vien-c/ham-fread-trong-c.jsp
		fread(colorTable, sizeof(unsigned char), 1024, fileIn);
	}

	// Bien de luu tru thong tin cac pixel cua hinh anh
	// Bien de luu tru thong tin cac pixel cua hinh anh
	unsigned char imageDataIn[width * height];

	fread(imageDataIn, sizeof(unsigned char), (height * width), fileIn);

	FILE *fileOut = fopen("D:\\0023_prjo_Embedded_Android\\70_member\\QuanNH\\OutputTraining\\Image\\boats_histogram.bmp", "wb");
	// Ghi thong tin header ra file moi
	fwrite(header, sizeof(unsigned char), 54, fileOut);
	// ghi thong tin mau
	if (bitDepth <= 8)
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
	}
	// Ghi thong tin hinh anh
	histogramEqualisation(fileOut, imageDataIn, width, height);

	fclose(fileIn);
	fclose(fileOut);
}
