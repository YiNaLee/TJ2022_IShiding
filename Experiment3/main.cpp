#include<fstream>
#include<iostream>
#include"bmp.h"
#include "DCT.h"
#include "STDM.h"
using namespace std;
#define _NO_ATTACK_ 1
#define _ATTACKED_ 0
int main()
{

	//��ȡbmp��DCT �任��Ƕ��
	STDM stdm;
	BMP LENA=stdm.reading_bmps();
	stdm.insetSave(LENA);
	//Ȼ����� ��lenaͼ�н����ˮӡTJLOGO��ͳ�ƴ����ʡ�����logoͼ
	if (_NO_ATTACK_) {
		stdm.inverseAll();
		//���������:
		cout << "������Ϊ:" << stdm.error_rate() << endl;
		stdm.saveLOGO();
	}
	if (_ATTACKED_) {
		stdm.attacked();
	}
	
	return 0;
}