#define  _CRT_SECURE_NO_WARNINGS
#include <fstream> 
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
//引入时间库 和 随机函数库 
#include<time.h>
#include<stdlib.h>

//产生高斯样本，以mu为均值，sigma为均方差
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
	//定义原始图像的宽和高 
	unsigned int Height = 0;
	unsigned int Width = 0;
	//定义循环变量 
	int i = 0;
	int j = 0;

	BITMAPFILEHEADER bmpfileheader; //文件头
	BITMAPINFOHEADER bmpinfoheader; //信息头
	//24位像素点RGB结构体
	typedef struct tagRGB
	{
		BYTE blue;
		BYTE green;
		BYTE red;
	}RGBDATA;

	FILE* fpin; //读取操作流
	FILE* fpout; //读出操作流

	fpin = fopen("q14.bmp", "rb");
	fread(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fpin);//读取文件头
	fread(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fpin);//读取信息头
	Height = bmpinfoheader.biHeight;
	Width = bmpinfoheader.biWidth;

	//动态创建二维数组
	RGBDATA** RGBin;
	RGBin = (RGBDATA**)malloc(sizeof(RGBDATA*) * Height);
	for (i = 0; i < Height; i++)
	{
		RGBin[i] = (RGBDATA*)malloc(sizeof(RGBDATA) * Width);
	}

	//读入像素信息
	for (i = 0; i < Height; i++)
	{
		fread(RGBin[i], sizeof(tagRGB), Width, fpin);
	}

	cout << "请输入高斯噪声的均值:" << endl;
	double mu=0;
	//cin >> mu;
	cout << "请输入高斯噪声的方差:" << endl;
	double sigma=10;
	//cin >> sigma;
	cout << "请输入高斯噪声的系数:" << endl;
	double k=1;
	//cin >> k;

	srand((unsigned)(time(NULL)));//调用srand是每次产生的随机数不同 

	//将原始图像加高斯噪声
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			RGBin[j][i].blue += (k * generateGaussianNoise(mu, sigma));
			RGBin[j][i].green += (k * generateGaussianNoise(mu, sigma));
			RGBin[j][i].red += (k * generateGaussianNoise(mu, sigma));
		}
	}

	//将信息写入文件
	fpout = fopen("attackq14.bmp", "wb");
	fwrite(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fpout);
	fwrite(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fpout);
	for (i = 0; i < Height; i++)
	{
		fwrite(RGBin[i], sizeof(tagRGB), Width, fpout);
	}
	printf("生成新图片成功!\n");
	fclose(fpin);
	fclose(fpout);
	return 0;
}
