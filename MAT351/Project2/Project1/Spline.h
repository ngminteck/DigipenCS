#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

class Spline {
public:
	/**
	 * This method calculates the second derivatives of the interpolating function
	 * at the tabulated points xi, with xi = (i, y[i]).
	 * Based on Numerical Recipes in C
	 * Optionally, the first derivative for the first and last point can be specified.
	 * Use 1e30 if not specified, for a natural cubic spline interpolation.
	 * @param y Array of y coordinates for cubic-spline interpolation.
	 * @param ypFirst Optional: first derivative in the first point.
	 * @param ypLast Optional: first derivative in the last point.
	 */
	void init(std::vector<float> y, float ypFirst = 1e30, float ypLast = 1e30);

	/**
	 * Returns a cubic-spline interpolated value y for the point between
	 * point (n, y[n]) and (n+1, y[n+1), with t ranging from 0 for (n, y[n])
	 * to 1 for (n+1, y[n+1]).
	 * @param n The start point.
	 * @param t The distance to the next point (0..1).
	 * @return A cubic-spline interpolated value.
	 */
	float fn(int n, float t);

	/**
	 * First derivative of fn.
	 */
	float fn1(int n, float t);

	/**
	 * Second derivative of fn.
	 */
	float fn2(int n, float t);

	int size() { return int(y.size()); }

private:
	void testParams(int& n, float& t);
	std::vector<float> y;
	std::vector<float> y2;
};

#endif
