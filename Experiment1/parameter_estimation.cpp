#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
#define N 10000
//double parameter_estimation(double )\

//参数估计
//用两种估计方法：矩估计和极大似然估计
using namespace std;
int ran_num::parameter_estimation()
{
    fstream parameter_estimation("parameter_estimation_all.txt", ios::out);//参数估计
    //高斯分布的参数估计：
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
    parameter_estimation << "高斯分布实验:" << endl;
    parameter_estimation << "实际μ为:" << Guassian_mu << endl;
    parameter_estimation << "估计值为:" << Ex << endl;
    parameter_estimation << "实际σ为:" << Guassian_xigamma << endl;
    parameter_estimation << "矩估计估计值为:" << sqrtDx << endl;

    cout << "高斯分布实验:" << endl;
    cout << "实际μ为:" << Guassian_mu << endl;
    cout << "估计值为:" << Ex << endl;
    cout << "实际σ为:" << Guassian_xigamma << endl;
    cout << "矩估计估计值为:" << sqrtDx << endl;
    //高斯分布还需要增加极大似然估计过程
    //极大似然，μ是相同的，还需要σ
    double temp_sum = 0;
    for (int i = 0; i < 10000; i++) {
        temp_sum += (ran_Gaussian[i] - Guassian_mu) * (ran_Gaussian[i] - Guassian_mu);

    }
    double MLE_xigamma = sqrt(temp_sum / (1.0 * N));
    parameter_estimation << "极大似然估计值为:" << MLE_xigamma << endl;
    cout << "极大似然估计值为:" << MLE_xigamma << endl;
    //指数分布的参数估计：
    double sum = 0;
    for (int i = 0; i < N; i++) {
        sum += ran_exp[i];
    }
    double estimation_beta = sum / (1.0 * N);
    parameter_estimation << "****************************************\n";
    parameter_estimation << "指数分布实验\n";
    parameter_estimation << "实际β为:" << Exp_beta << endl;
    parameter_estimation << "估计值为：" << estimation_beta << endl;
    parameter_estimation << "****************************************\n";
   
    cout << "****************************************\n";
    cout << "指数分布实验\n";
    cout << "实际β为:" << Exp_beta << endl;
    cout << "估计值为：" << estimation_beta << endl;
    cout << "****************************************\n";
    cout << "****************************************\n";
    cout << "GGD实验\n估计参数c：\n";
    double resGGDc = parameter_GGD();
    parameter_estimation << "****************************************\n";
    parameter_estimation << "GGD实验\n";
    parameter_estimation << "实际c为:" <<GGD_c<< endl;
    parameter_estimation << "估计值为：" << resGGDc << endl;
    parameter_estimation << "****************************************\n";

   
    cout << "实际c为:" << GGD_c << endl;
    cout << "估计值为：" << resGGDc << endl;
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
        cout << "\n需要给定c的区间，[a,b].请输入a b ：";
        
        cin >> a >> b;
        cout << "需要给定步长，请输入步长[0,1]的小数:";
        cin >> stepsize;
        c = Gamma(
            E_abs_x / sigma,
            a, b, stepsize
        );
        flag = (c == a || c == b);
    } 
    return c;
}