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
	vector<double>origin_pic;//ԭLENA
	vector<double>inset_pic;//Ƕ��ˮӡ���pixel
	vector<int> origin_logo;
	vector<int> inset_logo;
	vector<int> inverse_logo;//��ͼ���н����logo
	vector<double>attack_pic;//�������pixel
	vector<int>attack_logo;//�ܹ����Ľ������logo
	unsigned char* Buf;//BMP��ָ�룿
public:
	STDM();
	~STDM();
	BMP reading_bmps();
	int insetSave(BMP lena);
	int insetAll();
	int inset8(double pic[8][8],int b);
	int inverseAll();//����
	int  Detector8x8(double Matrix[8][8]);
	double error_rate();
	int saveLOGO();
	double attacked();
	//��ȡ22��ϵ��
	int w[22] = { 1,1,1,1,1,1,1,1,1,1,1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
};