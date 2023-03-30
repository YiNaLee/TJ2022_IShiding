#pragma once
#include<string.h>
#include<vector>
#include "DCT.h"

using namespace std;
class Watermark {
private:
 //int w[22];
	double strength;
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
	Watermark();
	~Watermark();
	int inset8(double pic[8][8], double strength, int b);//��8*8�Ŀ����Ƕ��
	int insetAll();
	int insetSave(BMP lena);
	BMP reading_bmps();
	//int gen_data();
	int inverseAll();//��ԭlogo
	double extract_LS(double Matrix[8][8]);
	//�����ʼ���
	int saveLOGO();
	double error_rate();
	double attack_error_rate();//�ܵ�������
	//���������Ҫ�Ľ��ĵط�������ֻ�;����Ϊ88��Ҫ�����ֻ�����
	int w[22] = { 1,1,1,1,1,1,1,1,1,1,1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
};
