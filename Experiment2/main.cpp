#include<fstream>
#include<iostream>
#include"bmp.h"
#include "DCT.h"
#include "Spread_spectrum.h"
#include "test.h"
//#define PicSIZE 512
//#define LogoSIZE 8
#define DEBUG 0

using namespace std;

int main()
{

	if (DEBUG) {
        test_dct();
	}
    Watermark W;
    BMP LENA = W.reading_bmps();
    W.insetSave(LENA);
    W.inverseAll();
    cout << "������Ϊ:" << W.error_rate() << endl;
    W.saveLOGO();
    //W.attack_error_rate();//���빥����� 
	return 0;
}


