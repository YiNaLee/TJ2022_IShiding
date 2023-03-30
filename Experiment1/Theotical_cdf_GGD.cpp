#include <iostream>
#include <random>
#include <fstream>
#include "ran_class.h"
#include <math.h>
#define ITMAX 100
#define EPS 3.0e-7
#define N 10000
#define FPMIN 1.0e-30
using namespace std;
//不完备的gamma函数

float gammp(float a, float x)
{
	void gcf(float* gammcf, float a, float x, float* gln);
	void gser(float* gamser, float a, float x, float* gln);
	void nrerror(char error_text[]);
	float gamser, gammcf, gln;

	//if (x < 0.0 || a <= 0.0) nrerror("Invalid arguments in routine gammp");
	if (x < (a + 1.0)) {
		gser(&gamser, a, x, &gln);
		return gamser;
	}
	else {
		gcf(&gammcf, a, x, &gln);
		return 1.0 - gammcf;
	}
}
float gammq(float a, float x)
{
	void gcf(float* gammcf, float a, float x, float* gln);
	void gser(float* gamser, float a, float x, float* gln);
	void nrerror(char error_text[]);
	float gamser, gammcf, gln;

	//if (x < 0.0 || a <= 0.0) nrerror("Invalid arguments in routine gammq");
	if (x < (a + 1.0)) {
		gser(&gamser, a, x, &gln);
		return 1.0 - gamser;
	}
	else {
		gcf(&gammcf, a, x, &gln);
		return gammcf;
	}
}
void gser(float* gamser, float a, float x, float* gln)
{
	float gammln(float xx);
	void nrerror(char error_text[]);
	int n;
	float sum, del, ap;

	*gln = gammln(a);
	if (x <= 0.0) {
		//if (x < 0.0) nrerror("x less than 0 in routine gser");
		*gamser = 0.0;
		return;
	}
	else {
		ap = a;
		del = sum = 1.0 / a;
		for (n = 1; n <= ITMAX; n++) {
			++ap;
			del *= x / ap;
			sum += del;
			if (fabs(del) < fabs(sum) * EPS) {
				*gamser = sum * exp(-x + a * log(x) - (*gln));
				return;
			}
		}
		//nrerror("a too large, ITMAX too small in routine gser");
		return;
	}
}

float gammln(float xx)
{
	double x, y, tmp, ser;
	static double cof[6] = { 76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5 };
	int j;

	y = x = xx;
	tmp = x + 5.5;
	tmp -= (x + 0.5) * log(tmp);
	ser = 1.000000000190015;
	for (j = 0; j <= 5; j++) ser += cof[j] / ++y;
	return -tmp + log(2.5066282746310005 * ser / x);
}

void gcf(float* gammcf, float a, float x, float* gln)
{
	float gammln(float xx);
	void nrerror(char error_text[]);
	int i;
	float an, b, c, d, del, h;

	*gln = gammln(a);
	b = x + 1.0 - a;
	c = 1.0 / FPMIN;
	d = 1.0 / b;
	h = d;
	for (i = 1; i <= ITMAX; i++) {
		an = -i * (i - a);
		b += 2.0;
		d = an * d + b;
		if (fabs(d) < FPMIN) d = FPMIN;
		c = b + an / c;
		if (fabs(c) < FPMIN) c = FPMIN;
		d = 1.0 / d;
		del = d * c;
		h *= del;
		if (fabs(del - 1.0) < EPS) break;
	}
	//if (i > ITMAX) nrerror("a too large, ITMAX too small in gcf");
	*gammcf = exp(-x + a * log(x) - (*gln)) * h;
}
#undef ITMAX
/*以上都是Numerical-Recipes-in-C中的源代码，需要用到的几个函数*/

/* GGD theoretical value
	return: x_arr and y_arr (CDF value)

	beta is scale param
	c is shape param
	size is x_arr or y_arr's size
	min is bound of GGD sample gain fron GGD_vector.
	so does max.
*/
std::pair<
	std::vector<double>,
	std::vector<double>
> GGD_theoretical_CDF(
	double beta, double c, double size,
	double min, double max
) {
	std::vector<double> x_arr, y_arr;
	const double stride = (max - min) / size;//步长
	for (size_t index = 0; index <= size; ++index)
		x_arr.push_back(min + stride * index);

	for (double x : x_arr) {
		double P_res;
		if (x < 0) {
			P_res = gammp(1.0 / c, pow(-x, c) * pow(beta, c));
			y_arr.push_back(0.5 * (1 - P_res));
		}
		else {
			P_res = gammp(1.0 / c, pow(x, c) * pow(beta, c));
			y_arr.push_back(0.5 * P_res + 0.5);
		}
	}

	return std::pair<
		std::vector<double>,
		std::vector<double>
	> {x_arr, y_arr};
}

void ran_num::Theotical_CDF()
{
	
	sort(ran_GGD, ran_GGD + N);
	double numbercdf = 0;
	double min_ggd = ran_GGD[0];
	double max_ggd = ran_GGD[N - 1];
	cout << "产生的随机数中，最小值是" << min_ggd << "，最大值是" << max_ggd << ".\n";
	cout << "请输入需要产生的cdf的组数(20个左右比较合理)：";
	cin >> numbercdf;
	std::pair<
		std::vector<double>,
		std::vector<double>
	> res{ GGD_theoretical_CDF(
		GGD_beta, GGD_c, numbercdf,
		min_ggd, max_ggd
	) };
	cout << "理论cdf计算完成，共" << numbercdf << "个，请在Theotical_GGD.txt文件中查看\n";
	std::ofstream outcdf("Theotical_GGD.txt");
	for (double elem : res.second)
		outcdf << elem << std::endl;//将理论cdf输出
	outcdf.close();
}