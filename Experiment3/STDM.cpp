#include<iostream>
#include "bmp.h"
#include "STDM.h"

#define PIC_size 512
#define LOGO_size 64
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


//���캯������ʼ��pixel�ռ�
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

//�������ܣ���ȡLENA��LOGO���ڴ�
BMP STDM::reading_bmps()
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
				inset_logo[7 - k + count * 8] = ((a >> k) & 0x01);//��ÿһλ����
			}
			count++;
		}
	//note������Ҫ�������ǰѶ��������ء�0-255����ɷֿ��Ķ�����
	//����һλ��ɰ�λ��û�еĻ������
	return lena;
}


//�������ܣ����ˮӡǶ�룬�����µ�ͼƬ��ȷ���Ѿ�readBMP�ɹ�֮��������
int STDM::insetSave(BMP lena)
{
	int cnt = 0;
	cout << "������������������";
	cin >> step_delta;
	cout << "��ʼǶ�����,��=" << step_delta << "\n";
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
	cout << "�޹���ʱ��STDMǶ����ͼƬ�����ɣ�ΪLenaInset_res.BMP\n";

	return 0;
}

//�ⲿ��û��
int STDM::insetAll()
{
	randomize(w, 22);//��w���н����ֻ�����
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
			//׼����8*8����Ƕ�� ��ǿ��׳�ԣ�һ��bǶ�뵽4��8*8������ȥ
			inset8(M8x8,insert_b);

			//todo������һ���ָı�д����ͼƬ��
			//����ȡ8*8����任
			for (int k = 0; k < 8; k++) {
				for (int t = 0; t < 8; t++) {
					inset_pic[(i + k) * PIC_size + j + t] = M8x8[k][t];

				}
			}

		}
	}
	return 0;
}

//�������ܣ���8*8��С�����ˮӡǶ��
//ѡ����DCT����������Խ���+�������Խ��� ��22��
//DCT���任ֵ������pic��
int STDM::inset8(double pic[8][8], int b)
{
	DCT d;
	memcpy(d.input, pic, sizeof(double) * N * N);
	d.DCT_transform();
	//��ȡDCTϵ��
	//ѡ��Ƶϵ����(�Խ���+��������)����Ƕ��
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
	//�ⲿ�ָ���Ϊ����ˮӡ��
	//Ϊ�˱�֤�㹻�Ľ�׳�ԣ���Ƶ�㷨ͨ��
	//��Ҫʹ�ö����N����ϵ����Ƕ��һλb
	double mean_X = 0;
	for (int ix = 0; ix < 22; ix++) {
		mean_X += zigzag[ix] * w[ix];
	}
	mean_X = mean_X / (22.0);
	
	for (int i = 0; i < 22; i++) {
		//zigzag[i] += strength * w[i] * (b ? 1 : -1);
		//s=x+b*a*wi(a��strengthǶ��ǿ��)
		double qbx;
		if (b == 1) {
			//q1x=��floor((x-d1)/��)+d1
			qbx = step_delta * floor((mean_X - step_delta / 4.0) / step_delta + 0.5) + step_delta / 4.0;
		}
		else {//b==0
			qbx= step_delta * floor((mean_X + step_delta / 4.0) / step_delta + 0.5) - step_delta / 4.0;
		}
		zigzag[i] += (qbx-mean_X)* w[i];
	}

	////////////////////////////////////////////////////

	//��任д��
	for (int i = 0; i < 7; i++) {
		d.dctres[i][6 - i] = zigzag[cnt++];
	}
	for (int i = 0; i < 8; i++) {
		d.dctres[i][7 - i] = zigzag[cnt++];
	}
	for (int i = 1; i < 8; i++) {
		d.dctres[i][8 - i] = zigzag[cnt++];
	}//Ƕ��
	d.DCT_inverse();
	memcpy(pic, d.idctres, sizeof(double) * N * N);//�ش�ֵ
	return 0;
}


//todo����inset��ͼƬ���н��룬��ԭLOGOͼ�񣬲��Ƚϴ�����
//���벽�裺ˮӡͼ��->DCT���任->����-->���������ݶ���LOGO
//������Ƕ�����任

//inverseAll�������ܣ�ʹ��������ؼ�����ж��Ƿ�Ƕ��
//�õ������ݼ���inverse_logo��
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

//����ķ�����ʵ�����һ���ģ������ǱȽ�q0x-x���/4�ľ���
int STDM::Detector8x8(double Matrix[8][8])
{
	//DCT�任����ȡ��Ƶϵ��
	//DCT�任��Matrix
	DCT inverse;
	memcpy(inverse.input, Matrix, sizeof(double) * 8 * 8);
	inverse.DCT_transform();
	//�任�����inverse.dctres����������������ؼ�������
	//LS=(1/N)*sum(Siwi);Si��DCT��Ƶ����ȡ22��
	//LS>1?Ƕ��1��Ƕ��0
	double Y = 0;
	//	inverse.dctres
		//��ȡDCTϵ��
		//ѡ��Ƶϵ����(�Խ���+��������)����Ƕ��

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

//todo��ͳ��ʹ��������ؼ�����жϵĽ����ԭʼ������Ƚ��ܴ�����
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
	//��һ������inverse_logoתΪpixel
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
	cout << "�����LOGO�����ɣ�ΪinverseLOGO_res.BMP\n";
	return 0;
}

//���빥����
double STDM::attacked()
{
	//���ܹ���ͼƬ��
	//��ȡLENA���ڴ�
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
	cout << " �ܹ�������룬error=" << error << endl;
	double res = error / (1.0 * LOGO_size * LOGO_size);
	cout << "�ܹ����󣬴�����Ϊ��" << res << endl;
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
