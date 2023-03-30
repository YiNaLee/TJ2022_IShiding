#include<iostream>
#include "bmp.h"
#include "Spread_spectrum.h"

#define PIC_size 512
#define LOGO_size 32
using namespace std;

//�ֻ�����
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
//end of�ֻ�����
//���캯������ʼ��pixel�ռ�
Watermark::Watermark()
{
	pixel = new unsigned char[PIC_size * PIC_size];
	logo_pixel = new unsigned char[LOGO_size * LOGO_size];
	//todo
}
Watermark::~Watermark()
{
	delete[]pixel;
	delete[]logo_pixel;
}


//�������ܣ���8*8��С�����ˮӡǶ��
//ѡ����DCT����������Խ���+�������Խ��� ��22��
//DCT���任ֵ������pic��
int Watermark::inset8(double pic[8][8], double strength, int b)
{
	DCT d;
	memcpy(d.input, pic, sizeof(double) * N * N);
	d.DCT_transform();
	//��ȡDCTϵ��
	//ѡ��Ƶϵ����(�Խ���+��������)����Ƕ��
	double zigzag[22];
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
	//Ϊ�˱�֤�㹻�Ľ�׳�ԣ���Ƶ�㷨ͨ��
	//��Ҫʹ�ö����N����ϵ����Ƕ��һλb
	for (int i = 0; i < 22; i++) {
		zigzag[i] += strength * w[i] * (b ? 1 : -1);
			//s=x+b*a*wi(a��strengthǶ��ǿ��)
	}
	for (int i = 0; i < 7; i++) {
		 d.dctres[i][6 - i]=zigzag[cnt++] ;
	}
	for (int i = 0; i < 8; i++) {
		d.dctres[i][7 - i]=zigzag[cnt++]  ;
	}
	for (int i = 1; i < 8; i++) {
		 d.dctres[i][8 - i]= zigzag[cnt++] ;
	}//Ƕ��
	d.DCT_inverse();
	memcpy(pic, d.idctres, sizeof(double) * N * N);//�ش�ֵ
	return 0;
}

int Watermark::insetAll()
{
	randomize(w, 22);//��w���н����ֻ�����
	double M8x8[8][8];
	int index_logo = 0;
	int insert_b = 0;
	//512*512 origin_pic inset 32*32
	int use_of_b=0;
	for (int i = 0; i < PIC_size; i += 8) {
		for (int j = 0; j < PIC_size; j += 8) {
			if (use_of_b % 4 == 0) {
				insert_b = inset_logo[index_logo++];
			}//��������b����
			use_of_b++;
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					M8x8[k][t] = origin_pic[(i + k) * PIC_size + j + t];
					
				}
			}
			//׼����8*8����Ƕ�� ��ǿ��׳�ԣ�һ��bǶ�뵽4��8*8������ȥ
			inset8(M8x8, strength, insert_b);

			//todo������һ���ָı�д����ͼƬ��
			//����ȡ8*8����任
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					 inset_pic[(i + k) * PIC_size + j + t]=M8x8[k][t] ;

				}
			}

		}
	}
	return 0;
}
//�������ܣ���ȡLENA��LOGO���ڴ�
BMP Watermark::reading_bmps()
{
	//��ȡLENA���ڴ�
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
	//��ȡLOGO���ڴ�
	BMP L;
	if (L.readBMP("tj-logo.bmp") == 0)
		cout << "read tj-logo.bmp sucess" << endl;
	inset_logo.resize(LOGO_size * LOGO_size);
	int count = 0;
	for (int i = 0; i < LOGO_size; i++)
		for (int j = 0; j < LOGO_size/8; j++)
		{
			int a;
			a = (int)(*L.pBmp++);
			origin_logo.push_back(a);
			for (int k = 0; k < 8; k++)
			{
				inset_logo[7 - k + count *8] = ((a >> k) & 0x01);//��ÿһλ����
			}
			count++;
		}
	//note������Ҫ�������ǰѶ��������ء�0-255����ɷֿ��Ķ�����
	//����һλ��ɰ�λ��û�еĻ������
	return lena;
}


//�������ܣ����ˮӡǶ�룬�����µ�ͼƬ��ȷ���Ѿ�readBMP�ɹ�֮��������

int Watermark::insetSave(BMP lena)
{
	int cnt = 0;
	cout << "������Ƕ���ǿ�ȣ�Խ��ʧ��Խ���ԣ���Ƕ��Ч���ã�";
	cin >> strength;
	cout << "��ʼǶ�����,strength="<<strength<<"\n";
	insetAll();
	//save inset_pic:
	for (int i = 0; i < PIC_size; i++) {
		for (int j = 0; j < PIC_size; j++) {
			pixel[cnt++] = (unsigned char)inset_pic[PIC_size * i + j];

		}
	}
	lena.saveBmp("LenaInset_res.BMP", pixel, lena.biWidth, 
		lena.biHeight, lena.biBitCount, lena.pCOLORTable);
	cout << "Ƕ����ͼƬ�����ɣ�ΪLenaInset_res.BMP\n";

	return 0;
}




//�������ܣ���ÿ��8x8�ľ�������ȡLS
//inset_picΪǶ���ͼ��
double Watermark::extract_LS(double Matrix[8][8])
{
	//DCT�任����ȡ��Ƶϵ��
	//DCT�任��Matrix
	DCT inverse;
	memcpy(inverse.input, Matrix, sizeof(double) * 8 * 8);
	inverse.DCT_transform();
	//�任�����inverse.dctres����������������ؼ�������
	//LS=(1/N)*sum(Siwi);Si��DCT��Ƶ����ȡ22��
	//LS>1?Ƕ��1��Ƕ��0
	double LS = 0;
	//	inverse.dctres
		//��ȡDCTϵ��
		//ѡ��Ƶϵ����(�Խ���+��������)����Ƕ��

	int cnt = 0;
	for (int i = 0; i < 7; i++) {
		LS += inverse.dctres[i][6 - i] * w[cnt++];
	}
	for (int i = 0; i < 8; i++) {
		LS += inverse.dctres[i][7 - i] * w[cnt++];
	}
	for (int i = 1; i < 8; i++) {
		LS += inverse.dctres[i][8 - i] * w[cnt++];
	}
	

	return LS;
}

//todo����inset��ͼƬ���н��룬��ԭLOGOͼ�񣬲��Ƚϴ�����
//���벽�裺ˮӡͼ��->DCT���任->����-->���������ݶ���LOGO
//������Ƕ�����任

//inverseAll�������ܣ�ʹ��������ؼ�����ж��Ƿ�Ƕ��
//�õ������ݼ���inverse_logo��
int Watermark::inverseAll()
{
	double M8x8[8][8];
	int index_logo = 0;
	double LS = 0;
	int cnt = 0;
	//512*512 origin_pic inset 32*32
	for (int i = 0; i < PIC_size; i += 8) {
		for (int j = 0; j < PIC_size; j += 8) {
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					M8x8[k][t] = inset_pic[(i + k) * PIC_size + j + t];

				}
			}
			LS += extract_LS(M8x8);
			cnt++;
			if (cnt % 4 == 0) {//�Ѿ���4��8*8����
				int b = LS > 0 ? 1 : 0;
				LS = 0;
				inverse_logo.push_back(b);
			}
			
			
			
		}
	}
	//cout << "debug:inverse_logo_size:"<<inverse_logo.size()<<endl;
	//cout << "debug:insert_logo_size:" << inset_logo.size() << endl;
	return 0;
}


//todo��ͳ��ʹ��������ؼ�����жϵĽ����ԭʼ������Ƚ��ܴ�����
double Watermark::error_rate()
{
	int error = 0;
	for (int i = 0; i <LOGO_size * LOGO_size; i++) {
		if (inset_logo[i] != inverse_logo[i]) {
			error++;
		}
	}
	cout <<"error="<< error << endl;
	double res = error / (1.0*LOGO_size * LOGO_size);
	return res;
}


int Watermark::saveLOGO()
{

	BMP logo;
	logo.readBMP("tj-logo.bmp");
	//��һ������inverse_logoתΪpixel
	int count = 0;
	for (int i = 0; i < LOGO_size*LOGO_size/8; i++)
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
	cout << "�����LOGO�����ɣ�ΪinverseLOGO_res.BMP\n";
	return 0;
}
double Watermark::attack_error_rate()
{
	//���ܹ���ͼƬ��
	//��ȡLENA���ڴ�
	BMP attack_lena;
	if (attack_lena.readBMP("attack.bmp") == 0) {
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
	double LS = 0;
	int cnt = 0;
	//512*512 origin_pic inset 32*32
	for (int i = 0; i < PIC_size; i += 8) {
		for (int j = 0; j < PIC_size; j += 8) {
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					M8x8[k][t] = attack_pic[(i + k) * PIC_size + j + t];

				}
			}
			LS += extract_LS(M8x8);
			cnt++;
			if (cnt % 4 == 0) {//�Ѿ���4��8*8����
				int b = LS > 0 ? 1 : 0;
				LS = 0;
				attack_logo.push_back(b);
			}



		}
	}
	//error_rate()compure
	int error = 0;
	for (int i = 0; i < LOGO_size * LOGO_size; i++) {
		if (inset_logo[i] != attack_logo[i]) {
			error++;
		}
	}
	cout << " �ܹ�������룬error=" << error << endl;
	double res = error / (1.0 * LOGO_size * LOGO_size);
	cout << "�ܹ����󣬴�����Ϊ��" <<res << endl;
	//����attack_logoͼ
	BMP logo;
	logo.readBMP("tj-logo.bmp");
	//��һ������inverse_logoתΪpixel
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
	cout << "�ܹ���������LOGO�����ɣ�Ϊattack_LOGO.BMP\n";
	return res;
}
