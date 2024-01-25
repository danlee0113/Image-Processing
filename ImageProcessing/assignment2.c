#include <stdio.h>
#include<windows.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)
int main(int argc, char* argv[]) {//addition
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile1 = NULL, * inputFile2 = NULL;
	int a = 2, b = 5;
	inputFile1 = fopen("AICenter.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile1);//���� ����κ� �о����
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	inputFile2 = fopen("filter.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile2);//���� ����κ� �о����
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile2);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;//�� �ȼ�-> 8bit ¥�� RGB �����ϱ� 24bit�� ����. stride�� RGB ����� ���� ����.
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	//�������� processing 512*512
	unsigned char* inputImg1 = NULL, * inputImg2 = NULL, * outputImg = NULL;
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	double Y1, Y2, Y;//Y==0 �̸� ��Ӱ�, Y=255�̸� ���.
	double Cb;
	double Cr;
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);
	//R=Y+1.402* Cr;G=Y-0.714*Cr-0.344*Cb;B=Y+1.772*Cb;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {//width�� �ȼ� ���� RGB�� �ϳ��� ���� ����� �ʺ�. ���������� ǥ���ѰŶ� outputImg[j][i]�� ���� ��.
			Y1 = 0.299 * inputImg1[j * stride + 3 * i + 2] + 0.587 * inputImg1[j * stride + 3 * i + 1] + 0.114 * inputImg1[j * stride + 3 * i + 0];
			Y2 = 0.299 * inputImg2[j * stride + 3 * i + 2] + 0.587 * inputImg2[j * stride + 3 * i + 1] + 0.114 * inputImg2[j * stride + 3 * i + 0];
			Y = Y1 / a + Y2 / b;
			outputImg[j * stride + 3 * i + 2] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg[j * stride + 3 * i + 1] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg[j * stride + 3 * i + 0] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
		}
	}
	FILE* outputFile = fopen("Output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg1);
	free(inputImg2);
	fclose(inputFile1);
	fclose(inputFile2);
	fclose(outputFile);
	return 0;
}