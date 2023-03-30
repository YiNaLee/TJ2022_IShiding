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
	vector<double>origin_pic;//原LENA
	vector<double>inset_pic;//嵌入水印后的pixel
	vector<int> origin_logo;
	vector<int> inset_logo;
	vector<int> inverse_logo;//在图像中解码出logo
	vector<double>attack_pic;//攻击后的pixel
	vector<int>attack_logo;//受攻击的解码出的logo
	unsigned char* Buf;//BMP的指针？
public:
	Watermark();
	~Watermark();
	int inset8(double pic[8][8], double strength, int b);//对8*8的块进行嵌入
	int insetAll();
	int insetSave(BMP lena);
	BMP reading_bmps();
	//int gen_data();
	int inverseAll();//还原logo
	double extract_LS(double Matrix[8][8]);
	//错误率计算
	int saveLOGO();
	double error_rate();
	double attack_error_rate();//受到攻击后
	//这个序列有要改进的地方：随机轮换;序列为88，要进行轮换操作
	int w[22] = { 1,1,1,1,1,1,1,1,1,1,1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
};
