#ifndef PROBLEM_H
#define PROBLEM_H

#include "mathutil.h"

class Problem {
public:
    virtual ~Problem() {}
    virtual int getPointCount() = 0;
    virtual void getRotationAndPosition(int point, float time, Vector3& r1, Vector3& r2, Vector3& r3, Vector3& position) = 0;
    virtual const char* getName() = 0;
};

#endif
