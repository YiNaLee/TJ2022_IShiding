#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <random>
using namespace std;

#define PI 3.14159
#define N 1000
#define STD 10
#define BIN 43//46个点
#define DECT 1000000
class MC {
public:
	double *X;//模拟宿主图像
	double *inset_S;//嵌入
	double *u_S;//不嵌入
	int *W;//W序列，
	int cnterror[BIN] = {0};
	int cntmiss[BIN] = {0};

	double pfa_theoretical[BIN];//理论误检率
	double pfa_experimental[BIN];//实验误检率
	double phi[BIN];//检测阈值，通ierfc得到
	double pm_theoretical[BIN];//理论漏检率
	double pm_experimental[BIN];
	//ierfc是用来获得检测阈值的
	double ierfc(const double y);//老师给的代码
	double inverse_Q(const double y);
	double Q(const double y);
	double cal_phi(const double pfa);//计算决策阈值phi
	double cal_pmtheory(const double pfa,const double a=1.8);//计算理论pm
	double cal_attack_pmtheory(const double pfa, const double a =1.8 ,const double STDV=2.5);
	//测试无攻击情况，c=2.0 a=1.8
	void test_no_attack(double a=1.8);
	void test_attack(double a = 1.8,double STDV=2.5);//有攻击
	double correlator(double* S);
	MC();
	~MC();

};

class gen_data {
public:
	double *data;//存放产生的数据（模拟宿主）；
	void gen_GGDone();
	void gen_GGDhalfone();
	void gen_GGDtwo();
	//void gen_gauss();//产生高斯分布随机数
	int Bernoulli_distribution();
	gen_data();
	~gen_data();
	//int insert_under_two();//c=2.0 a=1.8嵌入
	

};