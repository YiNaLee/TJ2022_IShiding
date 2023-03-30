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
	//��һ����Ϊλͼ�ļ�ͷBITMAPFILEHEADER��û����Ҫ����Ϣ
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);//skip
	BITMAPINFOHEADER head;//40�ֽ�
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	/*λͼ��Ϣͷ��40�ֽ�
DWORD  biSize;
LONG            biWidth;
LONG            biHeight;
WORD           biPlanes;������1�����ÿ���
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
//ÿ��������ռ�ֽڣ�4�ı�����
	int lineByte = (biWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256 
//ֻ�а�λɫͼ��Ҫ��ɫ��
	if (biBitCount == 8) {
		pCOLORTable = new RGBQUAD[256];//��ɫ������
		fread(pCOLORTable, sizeof(RGBQUAD), 256, fp);

	}
	if (biBitCount == 1) {
		pCOLORTable = new RGBQUAD[2];
		fread(pCOLORTable, sizeof(RGBQUAD), 2, fp);

	}
	pBmp = new unsigned char[lineByte * biHeight];
	fread(pBmp, 1, lineByte * biHeight * sizeof(unsigned char), fp);
	//��ϣ�
	fclose(fp);
	return 0;
}


//����һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ,����д��ָ���ļ���
bool BMP::saveBmp(const char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{

    //���λͼ����ָ��Ϊ0����û�����ݴ��룬��������

    if (!imgBuf)
        return 0;

    //��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0

    int colorTablesize = 0;

    if (biBitCount == 8)
        colorTablesize = 1024;

    //���洢ͼ������ÿ���ֽ���Ϊ4�ı���

    int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

    //�Զ�����д�ķ�ʽ���ļ�

    FILE* fp = fopen(bmpName, "wb");

    if (fp == 0)
        return 0;

    //����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ

    BITMAPFILEHEADER fileHead;

    fileHead.bfType = 0x4D42;//bmp����

    //bfSize��ͼ���ļ�4����ɲ���֮��

    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte * height;

    fileHead.bfReserved1 = 0;

    fileHead.bfReserved2 = 0;

    //bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��

    fileHead.bfOffBits = 54 + colorTablesize;

    //д�ļ�ͷ���ļ�

    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

    //����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ

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

    //дλͼ��Ϣͷ���ڴ�

    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

    //����Ҷ�ͼ������ɫ��д���ļ� 

    if (biBitCount == 8)
        fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

    //дλͼ���ݽ��ļ�
    else if (biBitCount == 1)
    {
        fwrite(pColorTable, sizeof(RGBQUAD), 2, fp);
    }//LOGOͼҪ��
    fwrite(imgBuf, height * lineByte, 1, fp);

    //�ر��ļ�

    fclose(fp);

    return 1;

}
