#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
/*产生符合高斯分布的随机数，然后
用参数估计法估计相应的，关于Box- Muller方法大家可以从网络或其他资料获
得，但是程序中一定要注明出处，表示对
他人成果的尊重。
可以直接利用产生[0, 1]均匀分布的类（C++标
准库中有这样的类）。
比较理论CDF和实验CDF*/
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
    cout << "请输入正态分布的参数mu : ";
    cin >> e;
    cout << "请输入正态分布的参数xigamma : ";
    cin >> sqrtvar;

}
int ran_num::Guassian()
{
    double* RanNumber = new double[N];
    ran_Gaussian = new double[N];
    if (RanNumber == NULL || ran_Gaussian == NULL) {
        cout << "动态内存申请失败！\n";
        exit(-1);
    }
    fstream Gaussoutfile("Random_Gauss.txt", ios::out);
    //将产生的正态分布的随机数输出到txt文件。
    get_parameter(Guassian_mu, Guassian_xigamma);
    Gaussoutfile <<"mu="<< Guassian_mu << endl;
    Gaussoutfile <<"xigamma="<< Guassian_xigamma << endl;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < N; i++) {
        double U1 = dis(gen);//一个【0,1】之间的均匀分布
        double U2 = dis(gen);
        RanNumber[i]= sqrt(-2 * log(1-U1)) * cos(2 * PI * U2);//避免log0 使用log(1-U1)
        ran_Gaussian[i] = Guassian_xigamma * RanNumber[i] + Guassian_mu;//res=xiggma*z+mu
        Gaussoutfile<< ran_Gaussian[i] << endl;
    }
    return 0;
}