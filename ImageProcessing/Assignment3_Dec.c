#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include<windows.h>

int main(int argc, char* argv[]) {
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;
    FILE* inputFile = NULL;
    inputFile = fopen("GateY.bmp", "rb"); //��� ��뿡�� �� bmp ���� �Է�
    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);


    unsigned char* inputImg = NULL, * outputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    double* Y1 = (double*)calloc(width * height, sizeof(double));
    int* Y2 = (int*)calloc(width * height, sizeof(int));

    fread(inputImg, sizeof(unsigned char), size, inputFile);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Y1[j * width + i] = inputImg[j * stride + 3 * i];
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    FILE* fp;//fixed-length code 3bit�� ������.(binarization ppt 5�ʿ� �־��� binary code �״�� ���.)
    int e;
    unsigned char bit[4] = { 0, };
    fp = fopen("bitstream.txt", "r"); //Encoder���� �� bitstream�� ������ ���� ����ϱ�. �б� ���� �ҷ��´�.
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            fread(bit, sizeof(unsigned char), 3, fp);//���ڿ� bit�� 3���� �о strcmp �Լ��� ���� ���ڿ� ��.

            if (strcmp(bit, "000") == 0) {
                e = -4;
            }
            else if (strcmp(bit, "001") == 0) {
                e = -3;
            }
            else if (strcmp(bit, "010") == 0) {
                e = -2;
            }
            else if (strcmp(bit, "011") == 0) {
                e = -1;
            }
            else if (strcmp(bit, "100") == 0) {
                e = 0;
            }
            else if (strcmp(bit, "101") == 0) {
                e = 1;
            }
            else if (strcmp(bit, "110") == 0) {
                e = 2;
            }
            else {// bit�� "111"�� ��ġ�ϴ� ���
                e = 3;
            }
            e *= 5;
            if (i == 0) {
                Y2[j * width + i] = e + 128;

            }
            else {
                Y2[j * width + i] = Y2[j * width + i - 1] + e;
            }

        }
    }
    fclose(fp);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            outputImg[j * stride + 3 * i + 2] = (unsigned char)(Y2[j * width + i] > 255 ? 255 : (Y2[j * width + i] < 0 ? 0 : Y2[j * width + i]));
            outputImg[j * stride + 3 * i + 1] = (unsigned char)(Y2[j * width + i] > 255 ? 255 : (Y2[j * width + i] < 0 ? 0 : Y2[j * width + i]));
            outputImg[j * stride + 3 * i + 0] = (unsigned char)(Y2[j * width + i] > 255 ? 255 : (Y2[j * width + i] < 0 ? 0 : Y2[j * width + i]));
        }
    }
    FILE* outputFile = fopen("reconDecY.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    free(inputImg);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}