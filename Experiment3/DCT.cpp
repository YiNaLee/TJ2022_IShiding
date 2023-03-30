#include "DCT.h"

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
                    temp += input[m][n] * cos((2.0 * m + 1) * k * PI / (2.0 * N)) * cos((2.0 * n + 1) * l * PI / (2.0 * N));
                }
            }
            dctres[k][l] = alpha * beta * temp;
            //around(alpha * beta * temp);
        }
    }

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

//DCT_inverse函数功能：把类变量DCT.dctres通过DCT逆变换转到DCT.idctres
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

                    temp += alpha * beta * dctres[k][l] * cos((2.0* m + 1) * k * PI / (2.0 * N)) * cos((2.0 * n + 1) * l * PI / (2.0 * N));

                }
            }
            idctres[m][n] = around(temp);//像素取整
        }
    }
}


