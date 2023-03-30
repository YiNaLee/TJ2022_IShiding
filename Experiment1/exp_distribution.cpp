#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
using namespace std;
#define N 10000
int ran_num::exp_distribution()
{
    fstream unformfile("Random_uniform.txt", ios::out);
    fstream Expoutfile("Random_Exp.txt", ios::out);
    ran_exp = new double[N];//��̬�ڴ�����
    if (ran_exp == NULL) {
        cout << "��̬�ڴ�����ʧ��!\n";
        exit(-1);
    }
    cout << "\n ���ò������ָ���ֲ��ĺ�������������Random_Exp.txt��\n";
    cout << "������ָ���ֲ���beta��ֵ ��";
    cin >> Exp_beta;
    if (Exp_beta <= 0) {
        cout << "ָ���ֲ��Ĳ���һ������0�����������Ĭ�ϲ���beta=1��\n";
        Exp_beta = 1;
    }
    Expoutfile << "Exp_beta=" << Exp_beta << endl;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < N; i++) {
        double u = dis(gen);//����һ��0-1�ľ��ȷֲ������
        unformfile << u << endl;
        ran_exp[i] = (-Exp_beta) * log(1 - u);
        Expoutfile << ran_exp[i] << endl;//�����ļ�
    }

    return 0;
}


