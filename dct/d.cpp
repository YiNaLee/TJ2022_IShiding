#include<math.h>
#include<iostream>
#define N 8
#define PI 3.1415926

class DCT {
public:
    double input[N][N];//原始
    double dctres[N][N];//正变换结果
    double idctres[N][N];//逆变换结果
    //double output[N][N];

    void DCT_transform();//正变换
    void DCT_inverse();//逆变换
    int around(double a);//修正
};

using namespace std;

void  DCT::DCT_transform() {

    double alpha, beta;//C(k)  C(l)
    int m = 0, n = 0, k = 0, l = 0;
    for (k = 0; k < N; k++)
    {
        for (l = 0; l < N; l++)
        {
            if (k == 0)
            {
                alpha = sqrt(1.0 / N);
            }
            else
            {
                alpha = sqrt(2.0 / N);
            }
            if (l == 0)
            {
                beta = sqrt(1.0 / N);
            }
            else
            {
                beta = sqrt(2.0 / N);
            }
            double temp = 0.0;
            for (m = 0; m < N; m++)
            {
                for (n = 0; n < N; n++)
                {
                    temp += input[m][n] * cos((2 * m + 1) * k * PI / (2.0 * N)) * cos((2 * n + 1) * l * PI / (2.0 * N));
                }
            }
            dctres[k][l] = alpha * beta * temp;
            //around(alpha * beta * temp);
        }
    }

    //参考链接：https ://blog.csdn.net/BigDream123/article/details/104395587
    //有所改动
    //用来修正数据

}

int DCT::around(double a)
{
    if (a >= 255)
        return 255;
    else if (a <= 0)
        return 0;
    else
        return int(a + 0.5);
}


void DCT::DCT_inverse()
{
    double alpha, beta;
    int m = 0, n = 0, k = 0, l = 0;
    for (m = 0; m < N; m++)
    {
        for (n = 0; n < N; n++)
        {
            double temp = 0.0;
            for (k = 0; k < N; k++)
            {
                for (l = 0; l < N; l++)
                {
                    if (k == 0)
                    {
                        alpha = sqrt(1.0 / N);
                    }
                    else
                    {
                        alpha = sqrt(2.0 / N);
                    }
                    if (l == 0)
                    {
                        beta = sqrt(1.0 / N);
                    }
                    else
                    {
                        beta = sqrt(2.0 / N);
                    }

                    temp += alpha * beta * dctres[k][l] * cos((2.0 * m + 1) * k * PI / (2 * N)) * cos((2 * n + 1) * l * PI / (2 * N));

                }
            }
            idctres[m][n] = around(temp);
        }
    }
}
int main()
{
   
    DCT test;
    short i = 0;
    short j = 0;
    short u = 0;
    short v = 0;
    // 8 x 8 的图像数据
    double ori_data[N][N] =
    {
              {89, 101, 114, 125, 126, 115, 105, 96},
              {97, 115, 131, 147, 149, 135, 123, 113},
              {114, 134, 159, 178, 175, 164, 149, 137},
              {121, 143, 177, 196, 201, 189, 165, 150},
              {119, 141, 175, 201, 207, 186, 162, 144},
              {107, 130, 165, 189, 192, 171, 144, 125},
              {97, 119, 149, 171, 172, 145, 117, 96},
              {88, 107, 136, 156, 155, 129, 97, 75}
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
    return 0;
}