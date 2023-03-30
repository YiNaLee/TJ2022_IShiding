#define _CRT_SECURE_NO_WARNINGS
#include "bmp.h"
//#define ERROR -1
using namespace std;
int BMP::readBMP(const char* bmpfilename)
{
	FILE* fp = fopen(bmpfilename, "rb");
	if (fp == NULL) {
		cout << "open " << bmpfilename << "failed\n";
		return ERROR;
	}
	//第一部分为位图文件头BITMAPFILEHEADER，没有需要的信息
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);//skip
	BITMAPINFOHEADER head;//40字节
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	/*位图信息头：40字节
DWORD  biSize;
LONG            biWidth;
LONG            biHeight;
WORD           biPlanes;必须是1，不用考虑
WORD           biBitCount //1 4 8 24
DWORD  biCompression;
DWORD  biSizeImage;=biWidth*biHeight
LONG            biXPelsPerMeter;
LONG            biYPelsPerMeter;
DWORD  biClrUsed;
DWORD  biClrImportant;*/
	biWidth = head.biWidth;
	biHeight = head.biHeight;
	biBitCount = head.biBitCount;
//每行像素所占字节（4的倍数）
	int lineByte = (biWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256 
//只有八位色图需要调色板
	if (biBitCount == 8) {
		pCOLORTable = new RGBQUAD[256];//调色板数组
		fread(pCOLORTable, sizeof(RGBQUAD), 256, fp);

	}
	if (biBitCount == 1) {
		pCOLORTable = new RGBQUAD[2];
		fread(pCOLORTable, sizeof(RGBQUAD), 2, fp);

	}
	pBmp = new unsigned char[lineByte * biHeight];
	fread(pBmp, 1, lineByte * biHeight * sizeof(unsigned char), fp);
	//完毕；
	fclose(fp);
	return 0;
}


//给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息,将其写到指定文件中
bool BMP::saveBmp(const char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{

    //如果位图数据指针为0，则没有数据传入，函数返回

    if (!imgBuf)
        return 0;

    //颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0

    int colorTablesize = 0;

    if (biBitCount == 8)
        colorTablesize = 1024;

    //待存储图像数据每行字节数为4的倍数

    int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

    //以二进制写的方式打开文件

    FILE* fp = fopen(bmpName, "wb");

    if (fp == 0)
        return 0;

    //申请位图文件头结构变量，填写文件头信息

    BITMAPFILEHEADER fileHead;

    fileHead.bfType = 0x4D42;//bmp类型

    //bfSize是图像文件4个组成部分之和

    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte * height;

    fileHead.bfReserved1 = 0;

    fileHead.bfReserved2 = 0;

    //bfOffBits是图像文件前3个部分所需空间之和

    fileHead.bfOffBits = 54 + colorTablesize;

    //写文件头进文件

    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

    //申请位图信息头结构变量，填写信息头信息

    BITMAPINFOHEADER head;

    head.biBitCount = biBitCount;

    head.biClrImportant = 0;

    head.biClrUsed = 0;

    head.biCompression = 0;

    head.biHeight = height;

    head.biPlanes = 1;

    head.biSize = 40;

    head.biSizeImage = lineByte * height;

    head.biWidth = width;

    head.biXPelsPerMeter = 0;

    head.biYPelsPerMeter = 0;

    //写位图信息头进内存

    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

    //如果灰度图像，有颜色表，写入文件 

    if (biBitCount == 8)
        fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

    //写位图数据进文件
    else if (biBitCount == 1)
    {
        fwrite(pColorTable, sizeof(RGBQUAD), 2, fp);
    }//LOGO图要用
    fwrite(imgBuf, height * lineByte, 1, fp);

    //关闭文件

    fclose(fp);

    return 1;

}
