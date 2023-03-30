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

	//读取bmp，DCT 变换，嵌入
	STDM stdm;
	BMP LENA=stdm.reading_bmps();
	stdm.insetSave(LENA);
	//然后解码 从lena图中解码出水印TJLOGO；统计错误率、保存logo图
	if (_NO_ATTACK_) {
		stdm.inverseAll();
		//计算错误率:
		cout << "错误率为:" << stdm.error_rate() << endl;
		stdm.saveLOGO();
	}
	if (_ATTACKED_) {
		stdm.attacked();
	}
	
	return 0;
}