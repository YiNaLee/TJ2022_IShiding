#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include <Windows.h>
class BMP {
private:
public:
	unsigned char* pBmp;//����ͼ�����ݵ�ָ��
	int biWidth;
	int biHeight;
	int biBitCount;
	/*typedef struct tagRGBQUAD {
    BYTE   rgbBlue;
    BYTE   rgbGreen;
    BYTE   rgbRed;
    BYTE   rgbReserved;//����ֵ����Ϊ0
} RGBQUAD;*/
	RGBQUAD* pCOLORTable;
	int Perpixel_bit;//ͼ������
	int readBMP(const char* bmpfilename);//��ȡ
	bool saveBmp(const char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable);//����

};

