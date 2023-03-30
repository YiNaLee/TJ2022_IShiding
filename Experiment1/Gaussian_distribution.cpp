#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
/*�������ϸ�˹�ֲ����������Ȼ��
�ò������Ʒ�������Ӧ�ģ�����Box- Muller������ҿ��Դ�������������ϻ�
�ã����ǳ�����һ��Ҫע����������ʾ��
���˳ɹ������ء�
����ֱ�����ò���[0, 1]���ȷֲ����ࣨC++��
׼�������������ࣩ��
�Ƚ�����CDF��ʵ��CDF*/
using namespace std;
#define PI 3.14159265
#define N 10000
ran_num::ran_num() 
{
    Guassian_mu = 0;
    Guassian_xigamma = 0;
    Exp_beta = 0;
    GGD_c = 0;

};
ran_num::~ran_num() 
{
    delete[]ran_Gaussian;
    delete[]ran_exp;
    delete[]ran_GGD;
    delete[]ran_GGD1;
    delete[]ran_GGD05;
    
};
void get_parameter(double &e, double &sqrtvar)
{
    cout << "��������̬�ֲ��Ĳ���mu : ";
    cin >> e;
    cout << "��������̬�ֲ��Ĳ���xigamma : ";
    cin >> sqrtvar;

}
int ran_num::Guassian()
{
    double* RanNumber = new double[N];
    ran_Gaussian = new double[N];
    if (RanNumber == NULL || ran_Gaussian == NULL) {
        cout << "��̬�ڴ�����ʧ�ܣ�\n";
        exit(-1);
    }
    fstream Gaussoutfile("Random_Gauss.txt", ios::out);
    //����������̬�ֲ�������������txt�ļ���
    get_parameter(Guassian_mu, Guassian_xigamma);
    Gaussoutfile <<"mu="<< Guassian_mu << endl;
    Gaussoutfile <<"xigamma="<< Guassian_xigamma << endl;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < N; i++) {
        double U1 = dis(gen);//һ����0,1��֮��ľ��ȷֲ�
        double U2 = dis(gen);
        RanNumber[i]= sqrt(-2 * log(1-U1)) * cos(2 * PI * U2);//����log0 ʹ��log(1-U1)
        ran_Gaussian[i] = Guassian_xigamma * RanNumber[i] + Guassian_mu;//res=xiggma*z+mu
        Gaussoutfile<< ran_Gaussian[i] << endl;
    }
    return 0;
}