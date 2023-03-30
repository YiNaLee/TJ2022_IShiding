#include<iostream>
#include "bmp.h"
#include "STDM.h"

#define PIC_size 512
#define LOGO_size 64
using namespace std;


//轮换函数
void swap(int* a, int* b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int randint(int a, int b) {
	return rand() % (b - a + 1) + a;
}

void randomize(int arr[], int len) {
	int i;
	int j;
	for (i = 0; i < len; i++) {
		j = randint(i, len - 1);
		swap(&arr[i], &arr[j]);
	}
}


//构造函数，初始化pixel空间
STDM::STDM()
{
	pixel = new unsigned char[PIC_size * PIC_size];
	logo_pixel = new unsigned char[LOGO_size * LOGO_size];
	//todo
}
STDM::~STDM()
{
	delete[]pixel;
	delete[]logo_pixel;
}

//函数功能：读取LENA和LOGO进内存
BMP STDM::reading_bmps()
{
	//读取LENA进内存
	BMP lena;
	if (lena.readBMP("LENA.BMP") == 0) {
		cout << "LENA.BMP read success!\n";
	}
	for (int i = 0; i < PIC_size; i++) {
		for (int j = 0; j < PIC_size; j++) {
			origin_pic.push_back(*lena.pBmp);
			inset_pic.push_back(*lena.pBmp);
			lena.pBmp++;
		}
	}
	//读取LOGO进内存
	BMP L;
	if (L.readBMP("tj-logo.BMP") == 0)
		cout << "read tj-logo.bmp sucess" << endl;
	inset_logo.resize(LOGO_size * LOGO_size);
	int count = 0;
	for (int i = 0; i < LOGO_size; i++)
		for (int j = 0; j < LOGO_size / 8; j++)
		{
			int a;
			a = (int)(*L.pBmp++);
			origin_logo.push_back(a);
			for (int k = 0; k < 8; k++)
			{
				inset_logo[7 - k + count * 8] = ((a >> k) & 0x01);//把每一位读出
			}
			count++;
		}
	//note：必须要，这里是把读到的像素【0-255】拆成分开的二进制
	//即把一位拆成八位，没有的话会出错
	return lena;
}


//函数功能：完成水印嵌入，生成新的图片，确保已经readBMP成功之后再做。
int STDM::insetSave(BMP lena)
{
	int cnt = 0;
	cout << "请输入量化步长△：";
	cin >> step_delta;
	cout << "开始嵌入过程,△=" << step_delta << "\n";
	insetAll();
	cout << "sucuess\n";
	//save inset_pic:
	for (int i = 0; i < PIC_size; i++) {
		for (int j = 0; j < PIC_size; j++) {
			pixel[cnt++] = (unsigned char)inset_pic[PIC_size * i + j];

		}
	}
	lena.saveBmp("LenaInset_res.BMP", pixel, lena.biWidth,
		lena.biHeight, lena.biBitCount, lena.pCOLORTable);
	cout << "无攻击时，STDM嵌入后的图片已生成，为LenaInset_res.BMP\n";

	return 0;
}

//这部分没变
int STDM::insetAll()
{
	randomize(w, 22);//对w序列进行轮换操作
	double M8x8[8][8];
	int index_logo = 0;
	int insert_b = 0;
	//512*512 origin_pic inset 64*64
	for (int i = 0; i < PIC_size; i += 8) {
		for (int j = 0; j < PIC_size; j += 8) {
			insert_b = inset_logo[index_logo++];
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					M8x8[k][t] = origin_pic[(i + k) * PIC_size + j + t];

				}
			}
			//准备对8*8进行嵌入 增强健壮性：一个b嵌入到4个8*8矩阵中去
			inset8(M8x8,insert_b);

			//todo：把这一部分改变写到新图片中
			//上面取8*8的逆变换
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					inset_pic[(i + k) * PIC_size + j + t] = M8x8[k][t];

				}
			}

		}
	}
	return 0;
}

//函数功能：对8*8的小块进行水印嵌入
//选的是DCT块里面的主对角线+两条副对角线 共22个
//DCT反变换值保存在pic中
int STDM::inset8(double pic[8][8], int b)
{
	DCT d;
	memcpy(d.input, pic, sizeof(double) * N * N);
	d.DCT_transform();
	//提取DCT系数
	//选中频系数主(对角线+两边两列)进行嵌入
	double zigzag[22] = {0};
	int cnt = 0;
	for (int i = 0; i < 7; i++) {
		zigzag[cnt++] = d.dctres[i][6 - i];
	}
	for (int i = 0; i < 8; i++) {
		zigzag[cnt++] = d.dctres[i][7 - i];
	}
	for (int i = 1; i < 8; i++) {
		zigzag[cnt++] = d.dctres[i][8 - i];
	}
	cnt = 0;
	//这部分更新为量化水印：
	//为了保证足够的健壮性，扩频算法通常
	//需要使用多个（N个）系数来嵌入一位b
	double mean_X = 0;
	for (int ix = 0; ix < 22; ix++) {
		mean_X += zigzag[ix] * w[ix];
	}
	mean_X = mean_X / (22.0);
	
	for (int i = 0; i < 22; i++) {
		//zigzag[i] += strength * w[i] * (b ? 1 : -1);
		//s=x+b*a*wi(a是strength嵌入强度)
		double qbx;
		if (b == 1) {
			//q1x=△floor((x-d1)/△)+d1
			qbx = step_delta * floor((mean_X - step_delta / 4.0) / step_delta + 0.5) + step_delta / 4.0;
		}
		else {//b==0
			qbx= step_delta * floor((mean_X + step_delta / 4.0) / step_delta + 0.5) - step_delta / 4.0;
		}
		zigzag[i] += (qbx-mean_X)* w[i];
	}

	////////////////////////////////////////////////////

	//逆变换写回
	for (int i = 0; i < 7; i++) {
		d.dctres[i][6 - i] = zigzag[cnt++];
	}
	for (int i = 0; i < 8; i++) {
		d.dctres[i][7 - i] = zigzag[cnt++];
	}
	for (int i = 1; i < 8; i++) {
		d.dctres[i][8 - i] = zigzag[cnt++];
	}//嵌入
	d.DCT_inverse();
	memcpy(pic, d.idctres, sizeof(double) * N * N);//回传值
	return 0;
}


//todo：对inset的图片进行解码，还原LOGO图像，并比较错误率
//解码步骤：水印图像->DCT正变换->解码-->解码后的数据读入LOGO
//解码是嵌入的逆变换

//inverseAll函数功能：使用线性相关检测器判断是否嵌入
//得到的数据加入inverse_logo中
int STDM::inverseAll()
{
	double M8x8[8][8];
	int index_logo = 0;
	//512*512 origin_pic inset 64*64
	for (int i = 0; i < PIC_size; i += 8) {
		for (int j = 0; j < PIC_size; j += 8) {
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					M8x8[k][t] = inset_pic[(i + k) * PIC_size + j + t];

				}
			}
			inverse_logo.push_back(Detector8x8(M8x8));
		}
	}
	//cout << "debug:inverse_logo_size:"<<inverse_logo.size()<<endl;
	//cout << "debug:insert_logo_size:" << inset_logo.size() << endl;
	return 0;
}

//解码的方法和实验二不一样的，这里是比较q0x-x与△/4的距离
int STDM::Detector8x8(double Matrix[8][8])
{
	//DCT变换，提取中频系数
	//DCT变换对Matrix
	DCT inverse;
	memcpy(inverse.input, Matrix, sizeof(double) * 8 * 8);
	inverse.DCT_transform();
	//变换结果在inverse.dctres，在其中用线性相关检测器检测
	//LS=(1/N)*sum(Siwi);Si是DCT中频，共取22个
	//LS>1?嵌入1：嵌入0
	double Y = 0;
	//	inverse.dctres
		//提取DCT系数
		//选中频系数主(对角线+两边两列)进行嵌入

	int cnt = 0;
	for (int i = 0; i < 7; i++) {
		Y += inverse.dctres[i][6 - i] * w[cnt++];
	}
	for (int i = 0; i < 8; i++) {
		Y += inverse.dctres[i][7 - i] * w[cnt++];
	}
	for (int i = 1; i < 8; i++) {
		Y+= inverse.dctres[i][8 - i] * w[cnt++];
	}
	Y = Y / 22.0;
	double d0 = -step_delta / 4.0;
	double q0Y = step_delta * floor((Y - d0) / step_delta + 0.5) + d0;
	if (abs(q0Y - Y) < step_delta / 4) 
		return 0;
	
	else return 1;
}

//todo：统计使用线性相关检测器判断的结果与原始结果，比较总错误率
double STDM::error_rate()
{
	int error = 0;
	for (int i = 0; i < LOGO_size * LOGO_size; i++) {
		if (inset_logo[i] != inverse_logo[i]) {
			error++;
		}
	}
	cout << "error=" << error << endl;
	double res = error / (1.0 * LOGO_size * LOGO_size);
	return res;
}

int STDM::saveLOGO()
{

	BMP logo;
	logo.readBMP("tj-logo.BMP");
	//第一步，把inverse_logo转为pixel
	int count = 0;
	for (int i = 0; i < LOGO_size * LOGO_size / 8; i++)
	{
		logo_pixel[count] = 0;
		for (int j = 0; j < 8; j++)
		{
			char c = 0;
			char c2;
			if (inverse_logo[7 - j + count * 8] == 1)
			{
				c2 = ((c >> j) | 0x01) << j;
				logo_pixel[count] |= c2;
			}
		}
		count++;
	}
	logo.saveBmp("inverseLOGO_res.BMP", logo_pixel, logo.biWidth,
		logo.biHeight, logo.biBitCount, logo.pCOLORTable);
	cout << "解码的LOGO已生成，为inverseLOGO_res.BMP\n";
	return 0;
}

//加入攻击后
double STDM::attacked()
{
	//打开受攻击图片；
	//读取LENA进内存
	BMP attack_lena;
	if (attack_lena.readBMP("q10_1.bmp") == 0) {
		cout << "attack.bmp read success!\n";
	}
	for (int i = 0; i < PIC_size; i++) {
		for (int j = 0; j < PIC_size; j++) {
			attack_pic.push_back(*attack_lena.pBmp);
			attack_lena.pBmp++;
		}
	}
	//inverse logo
	double M8x8[8][8];
	int index_logo = 0;
	int cnt = 0;
	//512*512 origin_pic inset 32*32
	for (int i = 0; i < PIC_size; i += 8) {
		for (int j = 0; j < PIC_size; j += 8) {
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					M8x8[k][t] = attack_pic[(i + k) * PIC_size + j + t];

				}
			}
			attack_logo.push_back(Detector8x8(M8x8));
		}
	}
	//error_rate()compure
	int error = 0;
	for (int i = 0; i < LOGO_size * LOGO_size; i++) {
		if (inset_logo[i] != attack_logo[i]) {
			error++;
		}
	}
	cout << " 受攻击后解码，error=" << error << endl;
	double res = error / (1.0 * LOGO_size * LOGO_size);
	cout << "受攻击后，错误率为：" << res << endl;
	//保存attack_logo图
	BMP logo;
	logo.readBMP("tj-logo.bmp");
	//第一步，把inverse_logo转为pixel
	int count = 0;
	for (int i = 0; i < LOGO_size * LOGO_size / 8; i++)
	{
		logo_pixel[count] = 0;
		for (int j = 0; j < 8; j++)
		{
			char c = 0;
			char c2;
			if (attack_logo[7 - j + count * 8] == 1)
			{
				c2 = ((c >> j) | 0x01) << j;
				logo_pixel[count] |= c2;
			}
		}
		count++;
	}
	logo.saveBmp("attack_LOGO.BMP", logo_pixel, logo.biWidth,
		logo.biHeight, logo.biBitCount, logo.pCOLORTable);
	cout << "受攻击后解码的LOGO已生成，为attack_LOGO.BMP\n";
	return res;
}
