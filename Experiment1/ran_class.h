#pragma once
/*��ͷ�ļ�����һ���࣬�����װ��main�����е���*/
#include<fstream>
class ran_num {
private:
	double* ran_Gaussian;//���ڶ�̬�ڴ����� 10000�������
	double* ran_exp;//���ڶ�̬�ڴ����� 10000�������
	double* ran_GGD;//ͨ��
	double* ran_GGD1;//����
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
	int GGD_distribution();//ͨ�ã����޶�cΪ1/0.5��c������������
	int parameter_estimation();
	double parameter_GGD();//GGD��������
	int GGD_c_is_one();//�޶�c=1.0
	int GGD_c_is_halfone();//�޶�c=0.5
	double Gamma(double target, double a, double b, double step = 0.01);
	void Theotical_CDF();
};