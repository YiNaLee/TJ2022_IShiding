#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
using namespace std;
#define N 10000
int ran_num::Bernoulli_distribution()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.5);
    int a = d(gen);
    return a;
}

/*E = Gamma(1/c, 1/(beta^c))^(1/c)  (X = 1)
        -Gamma(1/c, 1/(beta^c))^(1/c) (X = 0)*/
//ͨ�ã�c������������
int ran_num::GGD_distribution()
{
    ran_GGD = new double[N];
    if (ran_GGD == NULL) {
        cout << "��̬�ڴ�����ʧ�ܣ�\n";
        exit(-1);
    }
    fstream GGDoutfile("Random_GGD.txt", ios::out);
    //����������̬�ֲ�������������txt�ļ���
    
    //double beta;
    cout << "������GGD����״����c:";
    cin >> GGD_c;
    cout << "������GGD�ĳ߶Ȳ�����:";
    cin >> GGD_beta;
    GGDoutfile <<"GGD�Ĳ���c:"<<GGD_c << endl;
    GGDoutfile << "GGD�Ĳ�����:"<<GGD_beta << endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::gamma_distribution<double> G{ 1 / GGD_c, 1 / pow(GGD_beta, GGD_c) };
    //Gamma������ָ�������õ�������ֱ���ÿ⺯��
    //��Ŭ���ֲ���c++��׼�����У�ֱ����
    //std::gamma_distribution<double> G {1/c, 1/pow(beta, c)};
    
    for (int i = 0; i < N; i++) {
        double gamm = G(gen);
        if (Bernoulli_distribution()) {
            ran_GGD[i] = pow(gamm, 1 / GGD_c);
        }
        else {
            ran_GGD[i]= -1.0*pow(gamm, 1 / GGD_c);
        }
        GGDoutfile << ran_GGD[i] << endl;
    }
    return 0;
}

//��Ŭ���ֲ���Ȼ�ÿ⺯����gamma�������Բ��ã���ָ���ֲ�
int ran_num::GGD_c_is_one()
{
    
    fstream GGD1outfile("Random_GGD_cis_one.txt", ios::out);
    //�ȵ���ָ���ֲ�
    double beta = 1;
    cout << "GGD1,����c=1.0�����������beta��ֵ:";
    cin >> beta;
    if (beta <= 0) {
        cout << "ָ���ֲ��Ĳ���һ������0�����������Ĭ�ϲ���beta=1��\n";
        beta = 1;
    }
    GGD1outfile << "c=1.0,beta=" << beta << endl;
    double* exp1 = new double[N];
    
    if ( exp1 == NULL) {
        cout << "��̬�ڴ�����ʧ��\n";
        exit(-1);
    }
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < N; i++) {
        double u = dis(gen);//����һ��0-1�ľ��ȷֲ������
        exp1[i] = (-beta) * log(1 - u);
    }
    ran_GGD1 = new double[N];
    if (ran_GGD1 == NULL) {
        cout << "�ڴ�����ʧ��\n";
        exit(-1);
    }
    for (int i = 0; i < N; i++) {
        if (Bernoulli_distribution()) {
            ran_GGD1[i] = exp1[i];
        }
        else {
            ran_GGD1[i] = -exp1[i];
        }
        GGD1outfile <<ran_GGD1[i]<< endl;
    }
    delete[]exp1;
    return 0;
}



int ran_num::GGD_c_is_halfone()
{
    
    ran_GGD05 = new double[N];
    if (ran_GGD05 == NULL) {
        cout << "�ڴ�����ʧ��\n";
        exit(-1);
    }
    fstream GGD05outfile("Random_GGD_cis_05.txt", ios::out);
    //�ȵ���ָ���ֲ�
    double beta = 1;
    cout << "GGD1,����c=0.5�����������beta��ֵ:";
    cin >> beta;
    if (beta <= 0) {
        cout << "ָ���ֲ��Ĳ���һ������0�����������Ĭ�ϲ���beta=1��\n";
        beta = 1;
    }
    GGD05outfile << "c=0.5,beta=" << beta << endl;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    //��Ҫ��������ָ���ֲ�
  
    double* exp1 = new double[N];
    double* exp2 = new double[N];
    if (exp2 == NULL||exp1==NULL) {
        cout << "��̬�ڴ�����ʧ��\n";
        exit(-1);
    }
    for (int i = 0; i < N; i++) {
        double u1 = dis(gen);//����һ��0-1�ľ��ȷֲ������
        exp1[i] = (-beta) * log(1 - u1);
    }
    for (int i = 0; i < N; i++) {
        double u2 = dis(gen);//����һ��0-1�ľ��ȷֲ������
        exp2[i] = (-beta) * log(1 - u2);
    }
    //���϶����ԣ����
    /*�����������Y��Z�໥�����ҷ�����ͬ
��ָ���ֲ�(�ڴ˴��߶Ȳ���Ϊ1/��
0.5),
��ôY + Z�ͷ�����״����Ϊ2���߶Ȳ���
Ϊ 1/��
0.5��Gamma�ֲ�*/
    sort(exp2, exp2 + N);
    sort(exp1, exp1 +N);
    for (int i = 0; i < N; i++) {
        exp1[i] += exp2[i];//Y+Z Ϊgamma�ֲ�
        if (Bernoulli_distribution()) {
            ran_GGD05[i] = exp1[i] * exp1[i];
        }
        else {
            ran_GGD05[i] = exp1[i] * exp1[i] * (-1);
        }
        GGD05outfile << ran_GGD05[i] << endl;
    }
    delete[]exp1;
    delete[]exp2;
    return 0;
}



