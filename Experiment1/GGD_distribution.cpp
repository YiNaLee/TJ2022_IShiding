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
//通用，c可以任意输入
int ran_num::GGD_distribution()
{
    ran_GGD = new double[N];
    if (ran_GGD == NULL) {
        cout << "动态内存申请失败！\n";
        exit(-1);
    }
    fstream GGDoutfile("Random_GGD.txt", ios::out);
    //将产生的正态分布的随机数输出到txt文件。
    
    //double beta;
    cout << "请输入GGD的形状参数c:";
    cin >> GGD_c;
    cout << "请输入GGD的尺度参数β:";
    cin >> GGD_beta;
    GGDoutfile <<"GGD的参数c:"<<GGD_c << endl;
    GGDoutfile << "GGD的参数β:"<<GGD_beta << endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::gamma_distribution<double> G{ 1 / GGD_c, 1 / pow(GGD_beta, GGD_c) };
    //Gamma可以由指数函数得到，或者直接用库函数
    //贝努利分布在c++标准库中有，直接用
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

//伯努利分布仍然用库函数，gamma函数可以不用，用指数分布
int ran_num::GGD_c_is_one()
{
    
    fstream GGD1outfile("Random_GGD_cis_one.txt", ios::out);
    //先调用指数分布
    double beta = 1;
    cout << "GGD1,参数c=1.0，请输入参数beta的值:";
    cin >> beta;
    if (beta <= 0) {
        cout << "指数分布的参数一定大于0！输入错误，置默认参数beta=1！\n";
        beta = 1;
    }
    GGD1outfile << "c=1.0,beta=" << beta << endl;
    double* exp1 = new double[N];
    
    if ( exp1 == NULL) {
        cout << "动态内存申请失败\n";
        exit(-1);
    }
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < N; i++) {
        double u = dis(gen);//产生一个0-1的均匀分布随机数
        exp1[i] = (-beta) * log(1 - u);
    }
    ran_GGD1 = new double[N];
    if (ran_GGD1 == NULL) {
        cout << "内存申请失败\n";
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
        cout << "内存申请失败\n";
        exit(-1);
    }
    fstream GGD05outfile("Random_GGD_cis_05.txt", ios::out);
    //先调用指数分布
    double beta = 1;
    cout << "GGD1,参数c=0.5，请输入参数beta的值:";
    cin >> beta;
    if (beta <= 0) {
        cout << "指数分布的参数一定大于0！输入错误，置默认参数beta=1！\n";
        beta = 1;
    }
    GGD05outfile << "c=0.5,beta=" << beta << endl;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    //需要产生两个指数分布
  
    double* exp1 = new double[N];
    double* exp2 = new double[N];
    if (exp2 == NULL||exp1==NULL) {
        cout << "动态内存申请失败\n";
        exit(-1);
    }
    for (int i = 0; i < N; i++) {
        double u1 = dis(gen);//产生一个0-1的均匀分布随机数
        exp1[i] = (-beta) * log(1 - u1);
    }
    for (int i = 0; i < N; i++) {
        double u2 = dis(gen);//产生一个0-1的均匀分布随机数
        exp2[i] = (-beta) * log(1 - u2);
    }
    //符合独立性，相加
    /*假设随机变量Y和Z相互独立且服从相同
的指数分布(在此处尺度参数为1/β
0.5),
那么Y + Z就服从形状参数为2，尺度参数
为 1/β
0.5的Gamma分布*/
    sort(exp2, exp2 + N);
    sort(exp1, exp1 +N);
    for (int i = 0; i < N; i++) {
        exp1[i] += exp2[i];//Y+Z 为gamma分布
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



