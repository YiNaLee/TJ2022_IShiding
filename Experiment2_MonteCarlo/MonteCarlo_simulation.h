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
#define BIN 43//46����
#define DECT 1000000
class MC {
public:
	double *X;//ģ������ͼ��
	double *inset_S;//Ƕ��
	double *u_S;//��Ƕ��
	int *W;//W���У�
	int cnterror[BIN] = {0};
	int cntmiss[BIN] = {0};

	double pfa_theoretical[BIN];//���������
	double pfa_experimental[BIN];//ʵ�������
	double phi[BIN];//�����ֵ��ͨierfc�õ�
	double pm_theoretical[BIN];//����©����
	double pm_experimental[BIN];
	//ierfc��������ü����ֵ��
	double ierfc(const double y);//��ʦ���Ĵ���
	double inverse_Q(const double y);
	double Q(const double y);
	double cal_phi(const double pfa);//���������ֵphi
	double cal_pmtheory(const double pfa,const double a=1.8);//��������pm
	double cal_attack_pmtheory(const double pfa, const double a =1.8 ,const double STDV=2.5);
	//�����޹��������c=2.0 a=1.8
	void test_no_attack(double a=1.8);
	void test_attack(double a = 1.8,double STDV=2.5);//�й���
	double correlator(double* S);
	MC();
	~MC();

};

class gen_data {
public:
	double *data;//��Ų��������ݣ�ģ����������
	void gen_GGDone();
	void gen_GGDhalfone();
	void gen_GGDtwo();
	//void gen_gauss();//������˹�ֲ������
	int Bernoulli_distribution();
	gen_data();
	~gen_data();
	//int insert_under_two();//c=2.0 a=1.8Ƕ��
	

};