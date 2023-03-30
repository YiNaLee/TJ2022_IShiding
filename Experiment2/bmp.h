#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include <Windows.h>
class BMP {
private:
public:
	unsigned char* pBmp;//读入图像数据的指针
	int biWidth;
	int biHeight;
	int biBitCount;
	/*typedef struct tagRGBQUAD {
    BYTE   rgbBlue;
    BYTE   rgbGreen;
    BYTE   rgbRed;
    BYTE   rgbReserved;//保留值设置为0
} RGBQUAD;*/
	RGBQUAD* pCOLORTable;
	int Perpixel_bit;//图像类型
	int readBMP(const char* bmpfilename);//读取
	bool saveBmp(const char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable);//保存

};

