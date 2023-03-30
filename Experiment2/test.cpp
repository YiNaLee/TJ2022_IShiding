#include "test.h"
#define LOGO_size 32
/*int Watermark::gen_data()
{
	BMP lena;
	if (lena.readBMP("LENA.BMP") == 0) {
		cout << "LENA.BMP read success!\n";
	}

	//读取LOGO进内存
	BMP L;
	if (L.readBMP("tj-logo.bmp") == 0)
		cout << "read tj-logo.bmp sucess" << endl;



	for (int i = 0; i < PIC_size; i++) {
		for (int j = 0; j < PIC_size; j++) {
			origin_pic.push_back(*lena.pBmp);
			inset_pic.push_back(*lena.pBmp);
			lena.pBmp++;
		}
	}
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

	fstream out("datas.txt", ios::out);

	int cnt = 0;
	for (double i = 0; i < 10; i += 0.1) {
		strength = i;
		out << "strength" << "                      " << "error rate\n";
		out << strength << "                             ";
		insetAll();
		int error = 0;
		for (int i = 0; i < LOGO_size * LOGO_size; i++) {
			if (inset_logo[i] != inverse_logo[i]) {
				error++;
			}
		}
		double res = error / (1.0 * LOGO_size * LOGO_size);
		out << res << endl;

	}


	return 0;
}*/
int test_bmp()
{
    BMP lena;
    if (lena.readBMP("tj-logo.BMP") == 0) {
        cout << "read success!\n";
    }
    if (lena.saveBmp("tj-logoAnother.BMP", lena.pBmp, lena.biWidth,
        lena.biHeight, lena.biBitCount, lena.pCOLORTable) == 1) {
        cout << "\nwrite success!\n";
    }
    return 0;

}
int test_dct()
{
    //dct变换TESTBEGIN
    if (1)
    {
        DCT test;
        short i = 0;
        short j = 0;
        short u = 0;
        short v = 0;
        // 8 x 8 的图像数据
        double ori_data[N][N] =
        {
                  {154, 123, 123, 123, 123, 123, 123, 136},
                  {192, 180, 136, 154, 154, 154, 136, 110},
                  {254, 198, 154, 154, 180, 154, 123, 123},
                  {239, 180, 136, 180, 180, 166, 123, 123},
                  {180, 154, 136, 167, 166, 149, 136, 136},
                  {128, 136, 123, 136, 154, 180, 198, 154},
                  {123, 105, 110, 149, 136, 136, 180, 166},
                  {110, 136, 123, 123, 123, 136, 154, 136}
        };
        memset(test.input, 0, N * N * sizeof(double));
        memcpy(test.input, ori_data, N * N * sizeof(double));
        test.DCT_transform();

        cout << "The result of DCT:" << endl;

        for (u = 0; u < N; u++)
        {
            for (v = 0; v < N; v++)
            {
                cout << test.dctres[u][v] << "   ";
            }
            cout << endl;
        }

        test.DCT_inverse();
        cout << "The result of IDCT:" << endl;
        for (i = 0; i < N; i++)
        {
            for (short j = 0; j < N; j++)
            {
                cout << test.idctres[i][j] << "  ";
            }
            cout << endl;
        }
    }
    return 0;
}