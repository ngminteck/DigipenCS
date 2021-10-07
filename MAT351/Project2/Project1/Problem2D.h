#ifndef PROBLEM_2D_H
#define PROBLEM_2D_H

#include "Problem.h"
#include "Spline.h"

class Problem2D : public Problem {
public:
    Problem2D();
    int getPointCount() { return pointCount; }
    void getRotationAndPosition(int startPointIndex, float time, Vector3& r1, Vector3& r2, Vector3& r3, Vector3& position);
    const char* getName() { return "2D"; }

private:
    int pointCount;
    Spline splineX;
    Spline splineY;
};

#endif
