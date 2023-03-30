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
    ran_exp = new double[N];//动态内存申请
    if (ran_exp == NULL) {
        cout << "动态内存申请失败!\n";
        exit(-1);
    }
    cout << "\n 调用产生随机指数分布的函数，结果输出到Random_Exp.txt中\n";
    cout << "请输入指数分布的beta的值 ：";
    cin >> Exp_beta;
    if (Exp_beta <= 0) {
        cout << "指数分布的参数一定大于0！输入错误，置默认参数beta=1！\n";
        Exp_beta = 1;
    }
    Expoutfile << "Exp_beta=" << Exp_beta << endl;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    for (int i = 0; i < N; i++) {
        double u = dis(gen);//产生一个0-1的均匀分布随机数
        unformfile << u << endl;
        ran_exp[i] = (-Exp_beta) * log(1 - u);
        Expoutfile << ran_exp[i] << endl;//存入文件
    }

    return 0;
}


