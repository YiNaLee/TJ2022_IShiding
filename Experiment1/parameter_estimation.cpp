#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
#define N 10000
//double parameter_estimation(double )\

//��������
//�����ֹ��Ʒ������ع��ƺͼ�����Ȼ����
using namespace std;
int ran_num::parameter_estimation()
{
    fstream parameter_estimation("parameter_estimation_all.txt", ios::out);//��������
    //��˹�ֲ��Ĳ������ƣ�
    double sum_Gaussian = 0;
    double square_sum_Gaussian = 0;
    double sum_Exp = 0;
    for (int i = 0; i < N; i++) {
        sum_Gaussian += ran_Gaussian[i];
        square_sum_Gaussian += ran_Gaussian[i] * ran_Gaussian[i];
        sum_Exp += ran_exp[i];
        
    }
    double Ex = sum_Gaussian / (1.0 * N);
    double Ex2 = square_sum_Gaussian / (1.0 * N);
    double sqrtDx = sqrt(Ex2 - Ex * Ex);
    parameter_estimation << "��˹�ֲ�ʵ��:" << endl;
    parameter_estimation << "ʵ�ʦ�Ϊ:" << Guassian_mu << endl;
    parameter_estimation << "����ֵΪ:" << Ex << endl;
    parameter_estimation << "ʵ�ʦ�Ϊ:" << Guassian_xigamma << endl;
    parameter_estimation << "�ع��ƹ���ֵΪ:" << sqrtDx << endl;

    cout << "��˹�ֲ�ʵ��:" << endl;
    cout << "ʵ�ʦ�Ϊ:" << Guassian_mu << endl;
    cout << "����ֵΪ:" << Ex << endl;
    cout << "ʵ�ʦ�Ϊ:" << Guassian_xigamma << endl;
    cout << "�ع��ƹ���ֵΪ:" << sqrtDx << endl;
    //��˹�ֲ�����Ҫ���Ӽ�����Ȼ���ƹ���
    //������Ȼ��������ͬ�ģ�����Ҫ��
    double temp_sum = 0;
    for (int i = 0; i < 10000; i++) {
        temp_sum += (ran_Gaussian[i] - Guassian_mu) * (ran_Gaussian[i] - Guassian_mu);

    }
    double MLE_xigamma = sqrt(temp_sum / (1.0 * N));
    parameter_estimation << "������Ȼ����ֵΪ:" << MLE_xigamma << endl;
    cout << "������Ȼ����ֵΪ:" << MLE_xigamma << endl;
    //ָ���ֲ��Ĳ������ƣ�
    double sum = 0;
    for (int i = 0; i < N; i++) {
        sum += ran_exp[i];
    }
    double estimation_beta = sum / (1.0 * N);
    parameter_estimation << "****************************************\n";
    parameter_estimation << "ָ���ֲ�ʵ��\n";
    parameter_estimation << "ʵ�ʦ�Ϊ:" << Exp_beta << endl;
    parameter_estimation << "����ֵΪ��" << estimation_beta << endl;
    parameter_estimation << "****************************************\n";
   
    cout << "****************************************\n";
    cout << "ָ���ֲ�ʵ��\n";
    cout << "ʵ�ʦ�Ϊ:" << Exp_beta << endl;
    cout << "����ֵΪ��" << estimation_beta << endl;
    cout << "****************************************\n";
    cout << "****************************************\n";
    cout << "GGDʵ��\n���Ʋ���c��\n";
    double resGGDc = parameter_GGD();
    parameter_estimation << "****************************************\n";
    parameter_estimation << "GGDʵ��\n";
    parameter_estimation << "ʵ��cΪ:" <<GGD_c<< endl;
    parameter_estimation << "����ֵΪ��" << resGGDc << endl;
    parameter_estimation << "****************************************\n";

   
    cout << "ʵ��cΪ:" << GGD_c << endl;
    cout << "����ֵΪ��" << resGGDc << endl;
    cout << "****************************************\n";
    return 0;
}


double ran_num::Gamma( double target, double a, double b,double step) {
    double c = a;          
    double delta = target; 

    int cnt = 0;
    double c_i = a;    
    while (c_i <= b) {
        double expr = tgamma(2 / c_i) / sqrt(tgamma(1 / c_i) * tgamma(3 / c_i));
        double tmp_delta = fabs(target - expr);
        if (delta > tmp_delta) {
            delta = tmp_delta;
            c = a + cnt * step;
        }
        ++cnt;
        c_i = a + cnt * step;
    }
    return c;
}
double ran_num::parameter_GGD()
{
    double sum_square = 0, sum_abs = 0;
    for (int i = 0; i < N; i++) {
        sum_square += ran_GGD[i] * ran_GGD[i];
        sum_abs += fabs(ran_GGD[i]);
    }
    
    double sigma = sqrt(sum_square / N);
    double E_abs_x = sum_abs/N;

    bool flag = true;
    double c;
    while (flag) {
        double a, b, stepsize;
        cout << "\n��Ҫ����c�����䣬[a,b].������a b ��";
        
        cin >> a >> b;
        cout << "��Ҫ���������������벽��[0,1]��С��:";
        cin >> stepsize;
        c = Gamma(
            E_abs_x / sigma,
            a, b, stepsize
        );
        flag = (c == a || c == b);
    } 
    return c;
}