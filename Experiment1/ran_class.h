#pragma once
/*本头文件创建一个类，方便封装，main函数中调用*/
#include<fstream>
class ran_num {
private:
	double* ran_Gaussian;//用于动态内存申请 10000大的数组
	double* ran_exp;//用于动态内存申请 10000大的数组
	double* ran_GGD;//通用
	double* ran_GGD1;//特例
	double* ran_GGD05;
	double Guassian_mu;
	double Guassian_xigamma;
	double Exp_beta;
	double GGD_c;
	double GGD_beta;
public:
	ran_num();
	~ran_num();
	int Guassian();
	int exp_distribution();
	int Bernoulli_distribution();
	int GGD_distribution();//通用，不限定c为1/0.5，c可以任意输入
	int parameter_estimation();
	double parameter_GGD();//GGD参数估计
	int GGD_c_is_one();//限定c=1.0
	int GGD_c_is_halfone();//限定c=0.5
	double Gamma(double target, double a, double b, double step = 0.01);
	void Theotical_CDF();
};