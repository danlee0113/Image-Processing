//Upsampling a given Image. 
//input image size: 128*128, gray scale.
//upsampling to 512*512
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<windows.h>
int main(int argc, char* argv[]) {
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;
    FILE* inputFile = NULL;
    inputFile = fopen("test.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);



    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    int ratio = 2;
    int width2 = bmpInfo.biWidth << ratio;
    int height2 = bmpInfo.biHeight << ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2;
    unsigned char* inputImg = NULL, * outputImg = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));
    double* Y1 = (double*)calloc(width * height, sizeof(double));
    double* Y2 = (double*)calloc(width2 * height2, sizeof(double));
    fread(inputImg, sizeof(unsigned char), size, inputFile);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Y1[j * width + i] = inputImg[j * stride + 3 * i];
        }
    }
    /*
   ���� ����
   -> downsampling �� �̹������� �ϳ��� �ȼ��� �������� �����ϰڴ�. �ϳ��� �ȼ��� upsampling �Ǿ� 16���� �ȼ��� �ǹǷ�, �ϳ��� �ȼ��� 
    a b c d
    e f g h
    i j k l
    m n o p 
    �� ���� upsampling �Ǿ��ٰ� �����ϰ�, ���� a~p�� �̿��� �ش� �ڵ尡 � �ȼ����� ��Ÿ������ �����ϰڴ�.
    */
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
    for (int j = 0; j < height; j++) {//128*128�� downsampling �� �ȼ����� ���� �־��ֱ�. (a �ȼ� �ڸ�)
        for (int i = 0; i < width; i++) {
            Y2[(4 * j) * width2 + 4 * i] = Y1[j * width + i];
        }
    }
    for (int j = 0; j < height; j++) {//������ a �ڸ��� �ִ� ������ �� �־��־����Ƿ�, �¿쿡 �̿��� a���� ����� �̿��� c�� �ڸ� ���.
        for (int i = 0; i < width; i++) {
            if (i == width - 1) {
                Y2[(4 * j) * width2 + 4 * i + 2] = Y1[j * width + i];//�� ���� ���� �����ʿ� a�� �����Ƿ�, �׳� ������ a���� c �ڸ��� �־��ش�.
            }
            else {
                Y2[(4 * j) * width2 + 4 * i + 2] = Y1[j * width + i] * 0.5 + Y1[j * width + i + 1] * 0.5;
            }
        }
    }

    for (int j = 0; j < height; j++) {//��,�Ʒ� �̿��� a���� ����� �̿��� i �ڸ��� �ȼ��� ���.
        for (int i = 0; i < width; i++) {
            if (j == height - 1) {
                Y2[(4 * j + 2) * width2 + 4 * i] = Y1[j * width + i];//�� �Ʒ� ���� �� �̻� a���� �����Ƿ�, �׳� ������ a���� i �ڸ��� �־��ش�.
            }
            else {
                Y2[(4 * j + 2) * width2 + 4 * i] = Y1[j * width + i] * 0.5 + Y1[(j + 1) * width + i] * 0.5;
            }
        }
    }

    for (int j = 0; j < height; j++) {//�� ���� i���� ����� �̿��� k �ڸ��� �ȼ� �� ��� 
        for (int i = 0; i < width; i++) {
            if (i == width - 1) {
                Y2[(4 * j + 2) * width2 + 4 * i + 2] = Y1[j * width + i];//���� ������ ���� boundary processing �� ��.
            }
            else {
                Y2[(4 * j + 2) * width2 + 4 * i + 2] = Y2[(4 * j + 2) * width2 + 4 * i] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * (i + 1)] * 0.5;
            }
        }
    }
    for (int j = 0; j < height; j++) {// ���� b,d �ڸ��� �ȼ� ���.(���η� �̿��� �ȼ����� �̿�)-b,d �ڸ� ��� ���� ����� a�� c�� ����� �̿��� �ȼ� ���� ����Ѵ�.
        for (int i = 0; i < width; i++) {
            if (i == width - 1) {// ���� ������ ���� boundary processing �� ��.
                Y2[(4 * j) * width2 + 4 * i + 1] = Y1[j * width + i];//b �ڸ� boundary processing
                Y2[(4 * j) * width2 + 4 * i + 3] = Y1[j * width + i];//d �ڸ� boundary processing
            }
            else {
                Y2[(4 * j) * width2 + 4 * i + 1] = Y1[j * width + i] * 0.5 + Y2[(4 * j) * width2 + 4 * i + 2] * 0.5;//b �ڸ�
                Y2[(4 * j) * width2 + 4 * i + 3] = Y1[j * width + i + 1] * 0.5 + Y2[(4 * j) * width2 + 4 * i + 2] * 0.5;//d �ڸ�
            }
        }
    }
    for (int j = 0; j < height; j++) {//���� j,l �ڸ��� �ȼ��� ���.(���η� �̿��� �ȼ����� �̿�)- j,l �ڸ� ��� ����� i,k ����� �̿��� �ȼ� ���� ����Ѵ�.
        for (int i = 0; i < width; i++) {
            if (i == width - 1) {
                Y2[(4 * j + 2) * width2 + 4 * i + 1] = Y1[j * width + i];//j �ڸ� boundary processing
                Y2[(4 * j + 2) * width2 + 4 * i + 3] = Y1[j * width + i];//l �ڸ� boundary processing
            }
            else {
                Y2[(4 * j + 2) * width2 + 4 * i + 1] = Y2[(4 * j + 2) * width2 + 4 * i] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * i + 2] * 0.5;//j�ڸ� 
                Y2[(4 * j + 2) * width2 + 4 * i + 3] = Y2[(4 * j + 2) * width2 + 4 * i + 2] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * (i + 1)] * 0.5;//l �ڸ� 
            }
        }
    }

    for (int j = 0; j < height; j++) {//���� �Ʒ��� �̿��� i,a���� ����� �̿��� (���η� �̿��� �ȼ����� �̿�) e,m �ڸ��� �ȼ����� ���.
        for (int i = 0; i < width; i++) {
            if (j == height - 1) {
                Y2[(4 * j + 1) * width2 + 4 * i] = Y1[j * width + i];//e�ڸ� boundary processing
                Y2[(4 * j + 3) * width2 + 4 * i] = Y1[j * width + i];//m�ڸ� boundary processing
            }
            else {
                Y2[(4 * j + 1) * width2 + 4 * i] = Y1[j * width + i] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * i] * 0.5;//e�ڸ�
                Y2[(4 * j + 3) * width2 + 4 * i] = Y1[(j + 1) * width + i] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * i] * 0.5;//m�ڸ�
            }
        }
    }


    for (int j = 0; j < height; j++) {//���� f g h,n,o,p �ڸ��� �ȼ����� ���.
        for (int i = 0; i < width; i++) {
            if (j == height - 1) {//������� f,g,h,n,o,p �ڸ� boundary processing
                Y2[(4 * j + 1) * width2 + 4 * i + 1] = Y1[j * width + i];
                Y2[(4 * j + 1) * width2 + 4 * i + 2] = Y1[j * width + i];
                Y2[(4 * j + 1) * width2 + 4 * i + 3] = Y1[j * width + i];
                Y2[(4 * j + 3) * width2 + 4 * i + 1] = Y1[j * width + i];
                Y2[(4 * j + 3) * width2 + 4 * i + 2] = Y1[j * width + i];
                Y2[(4 * j + 3) * width2 + 4 * i + 3] = Y1[j * width + i];

            }
            else {
                Y2[(4 * j + 1) * width2 + 4 * i + 1] = Y2[(4 * j) * width2 + 4 * i + 1] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * i + 1] * 0.5;//�� �Ʒ��� �̿��� b,j�� ����� �̿��� f �ڸ��� �ȼ��� ���.
                Y2[(4 * j + 1) * width2 + 4 * i + 2] = Y2[(4 * j) * width2 + 4 * i + 2] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * i + 2] * 0.5;//�� �Ʒ��� �̿��� c,k�� ����� �̿��� g �ڸ��� �ȼ��� ���.
                Y2[(4 * j + 1) * width2 + 4 * i + 3] = Y2[(4 * j) * width2 + 4 * i + 3] * 0.5 + Y2[(4 * j + 2) * width2 + 4 * i + 3] * 0.5;//�� �Ʒ��� �̿��� d,l�� ����� �̿��� h �ڸ��� �ȼ��� ���.
                Y2[(4 * j + 3) * width2 + 4 * i + 1] = Y2[(4 * j + 2) * width2 + 4 * i + 1] * 0.5 + Y2[(4 * (j + 1)) * width2 + 4 * i + 1] * 0.5;//�� �Ʒ��� �̿��� j,b�� ����� �̿��� n �ڸ��� �ȼ��� ���.
                Y2[(4 * j + 3) * width2 + 4 * i + 2] = Y2[(4 * j + 2) * width2 + 4 * i + 2] * 0.5 + Y2[(4 * (j + 1)) * width2 + 4 * i + 2] * 0.5;//�� �Ʒ��� �̿��� k,c�� ����� �̿��� o �ڸ��� �ȼ��� ���.
                Y2[(4 * j + 3) * width2 + 4 * i + 3] = Y2[(4 * j + 2) * width2 + 4 * i + 3] * 0.5 + Y2[(4 * (j + 1)) * width2 + 4 * i + 3] * 0.5;//�� �Ʒ��� �̿��� l,d�� ����� �̿��� p �ڸ��� �ȼ��� ���.
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int j = 0; j < height2; j++) {
        for (int i = 0; i < width2; i++) {
            outputImg[j * stride2 + 3 * i + 2] = (unsigned char)(Y2[j * width2 + i] > 255 ? 255 : (Y2[j * width2 + i] < 0 ? 0 : Y2[j * width2 + i]));
            outputImg[j * stride2 + 3 * i + 1] = (unsigned char)(Y2[j * width2 + i] > 255 ? 255 : (Y2[j * width2 + i] < 0 ? 0 : Y2[j * width2 + i]));
            outputImg[j * stride2 + 3 * i + 0] = (unsigned char)(Y2[j * width2 + i] > 255 ? 255 : (Y2[j * width2 + i] < 0 ? 0 : Y2[j * width2 + i]));
        }
    }

    FILE* outputFile = fopen("OutputImg.bmp", "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

    free(outputImg);
    free(inputImg);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}