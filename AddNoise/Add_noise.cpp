#define  _CRT_SECURE_NO_WARNINGS
#include <fstream> 
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
//����ʱ��� �� ��������� 
#include<time.h>
#include<stdlib.h>

//������˹��������muΪ��ֵ��sigmaΪ������
double generateGaussianNoise(double mu, double sigma)
{
	static double V1, V2, S;
	static int phase = 0;
	double X;
	double U1, U2;
	if (phase == 0)
	{
		do
		{
			U1 = (double)rand() / RAND_MAX;
			U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
	{
		X = V2 * sqrt(-2 * log(S) / S);
	}
	phase = 1 - phase;
	return mu + sigma * X;
}

int main()
{
	//����ԭʼͼ��Ŀ�͸� 
	unsigned int Height = 0;
	unsigned int Width = 0;
	//����ѭ������ 
	int i = 0;
	int j = 0;

	BITMAPFILEHEADER bmpfileheader; //�ļ�ͷ
	BITMAPINFOHEADER bmpinfoheader; //��Ϣͷ
	//24λ���ص�RGB�ṹ��
	typedef struct tagRGB
	{
		BYTE blue;
		BYTE green;
		BYTE red;
	}RGBDATA;

	FILE* fpin; //��ȡ������
	FILE* fpout; //����������

	fpin = fopen("q14.bmp", "rb");
	fread(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fpin);//��ȡ�ļ�ͷ
	fread(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fpin);//��ȡ��Ϣͷ
	Height = bmpinfoheader.biHeight;
	Width = bmpinfoheader.biWidth;

	//��̬������ά����
	RGBDATA** RGBin;
	RGBin = (RGBDATA**)malloc(sizeof(RGBDATA*) * Height);
	for (i = 0; i < Height; i++)
	{
		RGBin[i] = (RGBDATA*)malloc(sizeof(RGBDATA) * Width);
	}

	//����������Ϣ
	for (i = 0; i < Height; i++)
	{
		fread(RGBin[i], sizeof(tagRGB), Width, fpin);
	}

	cout << "�������˹�����ľ�ֵ:" << endl;
	double mu=0;
	//cin >> mu;
	cout << "�������˹�����ķ���:" << endl;
	double sigma=10;
	//cin >> sigma;
	cout << "�������˹������ϵ��:" << endl;
	double k=1;
	//cin >> k;

	srand((unsigned)(time(NULL)));//����srand��ÿ�β������������ͬ 

	//��ԭʼͼ��Ӹ�˹����
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			RGBin[j][i].blue += (k * generateGaussianNoise(mu, sigma));
			RGBin[j][i].green += (k * generateGaussianNoise(mu, sigma));
			RGBin[j][i].red += (k * generateGaussianNoise(mu, sigma));
		}
	}

	//����Ϣд���ļ�
	fpout = fopen("attackq14.bmp", "wb");
	fwrite(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fpout);
	fwrite(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fpout);
	for (i = 0; i < Height; i++)
	{
		fwrite(RGBin[i], sizeof(tagRGB), Width, fpout);
	}
	printf("������ͼƬ�ɹ�!\n");
	fclose(fpin);
	fclose(fpout);
	return 0;
}
