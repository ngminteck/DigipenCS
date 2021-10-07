#ifndef PROBLEM_3D_H
#define PROBLEM_3D_H

#include "Problem.h"
#include "Spline.h"

class Problem3D : public Problem {
public:
    Problem3D();
    int getPointCount() { return pointCount; }
    void getRotationAndPosition(int startPointIndex, float time, Vector3& r1, Vector3& r2, Vector3& r3, Vector3& position);
    const char* getName() { return "3D"; }

private:
    int pointCount;
    Spline splineX;
    Spline splineY;
    Spline splineZ;
};

#endif
