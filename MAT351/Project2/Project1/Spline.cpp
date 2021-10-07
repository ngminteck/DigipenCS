#include "Spline.h"

using namespace std;

void Spline::init(std::vector<float> y, float ypFirst, float ypLast) {
	this->y = y;
	int n = int(y.size());
	y2 = vector<float>(n, 0);
	vector<float> u = vector<float>(n, 0);
	if (ypFirst < 0.99e30) {
		y2[0] = -0.5f;
		u[0] = 3.0f * ((y[1] - y[0]) - ypFirst);
	}
	for (int i = 1; i < n - 1; i++) {
		y2[i] = -1.0f / (4.0f + y2[i - 1]);
		u[i] = (6.0f * (y[i + 1] - 2.0f * y[i] + y[i - 1]) - u[i - 1]) / (4.0f + y2[i - 1]);
	}
	float qn = 0;
	float un = 0;
	if (ypLast < 0.99e30f) {
		qn = 0.5f;
		un = 3.0f * (ypLast - y[n - 1] + y[n - 2]);
	}
	y2[n - 1] = (un - qn * u[n - 2]) / (qn * y2[n - 2] + 1.0f);
	for (int i = n - 2; i >= 0; i--) {
		y2[i] = y2[i] * y2[i + 1] + u[i];
	}
}

void Spline::testParams(int& n, float& t) {
	if (n > int(y.size()) - 2) {
		n = int(y.size()) - 2;
		t = 1;
	}
	if (n < 0) {
		n = 0;
		t = 0;
	}
}

float Spline::fn(int n, float t) {
	testParams(n, t);
	if (n > int(y.size()) - 2) return 0;
	return t * y[n + 1] - ((t - 1.0f) * t * ((t - 2.0f) * y2[n] - (t + 1.0f) * y2[n + 1])) / 6.0f + y[n] - t * y[n];
}

float Spline::fn1(int n, float t) {
	testParams(n, t);
	if (n > int(y.size()) - 2) return 0;
	return y[n + 1] + y2[n] * (-0.5f * t * t + t - 1.0f / 3) + 0.5f * y2[n + 1] * t * t - y2[n + 1] / 6.0f - y[n];
}

float Spline::fn2(int n, float t) {
	testParams(n, t);
	if (n > int(y.size()) - 2) return 0;
	return y2[n] * (1.0f - t) + y2[n + 1] * t;
}
