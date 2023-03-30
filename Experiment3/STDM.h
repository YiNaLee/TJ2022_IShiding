#pragma once
#include<string.h>
#include<vector>
#include "bmp.h"
#include "DCT.h"

using namespace std;
class STDM {
private:
	//int w[22];
	double strength;
	double step_delta;
	unsigned char* pixel;
	unsigned char* logo_pixel;
	vector<double>origin_pic;//原LENA
	vector<double>inset_pic;//嵌入水印后的pixel
	vector<int> origin_logo;
	vector<int> inset_logo;
	vector<int> inverse_logo;//在图像中解码出logo
	vector<double>attack_pic;//攻击后的pixel
	vector<int>attack_logo;//受攻击的解码出的logo
	unsigned char* Buf;//BMP的指针？
public:
	STDM();
	~STDM();
	BMP reading_bmps();
	int insetSave(BMP lena);
	int insetAll();
	int inset8(double pic[8][8],int b);
	int inverseAll();//解码
	int  Detector8x8(double Matrix[8][8]);
	double error_rate();
	int saveLOGO();
	double attacked();
	//提取22个系数
	int w[22] = { 1,1,1,1,1,1,1,1,1,1,1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
};