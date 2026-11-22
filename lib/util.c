#include "util.h"

/**
    Solve Ax=y using Cramer's rule
*/
Vec3 solve_cramer(Mat3 A, Vec3 y) {
	float a = A.arr[0], d = A.arr[1], g = A.arr[2];
	float b = A.arr[3], e = A.arr[4], h = A.arr[5];
	float c = A.arr[6], f = A.arr[7], i = A.arr[8];

	float j = y.x, k = y.y, l = y.z;

	float M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);

	// Singular
	if (M == 0.0f)
		return (Vec3){0};

	Vec3 result;
	result.x =
	    (j * (e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g)) / M;

	result.y =
	    (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / M;

	result.z =
	    (j * (b * f - e * c) + k * (d * c - a * f) + l * (a * e - d * b)) / M;

	return result;
}

/**
    Solve Ax=y using Cramer's rule
    Computes only x[ind]
*/
float solve_cramerf(Mat3 A, Vec3 y, int ind) {
	float a = A.arr[0], d = A.arr[1], g = A.arr[2];
	float b = A.arr[3], e = A.arr[4], h = A.arr[5];
	float c = A.arr[6], f = A.arr[7], i = A.arr[8];

	float j = y.x, k = y.y, l = y.z;

	float M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);

	if (M == 0.0f)
		return NAN;

	switch (ind) {
	case 0: /* x */
		return (j * (e * i - h * f) + k * (g * f - d * i) +
		        l * (d * h - e * g)) /
		       M;

	case 1: /* y */
		return (i * (a * k - j * b) + h * (j * c - a * l) +
		        g * (b * l - k * c)) /
		       M;

	case 2: /* z */
		return (j * (b * f - e * c) + k * (d * c - a * f) +
		        l * (a * e - d * b)) /
		       M;

	default:
		return NAN;
	}
}