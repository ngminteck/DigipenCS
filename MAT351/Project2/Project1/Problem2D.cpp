#include "Problem2D.h"

#include <fstream>
#include <iostream>

static const char* inPath = "sample2d.txt";
static const char* outPath = "output1.txt";

Problem2D::Problem2D() {
    // open and read file
    std::ifstream inputFile(inPath);
    if (!inputFile.is_open()) {
        std::cout << "ERROR at 1: Could not open file " << inPath << std::endl;
        return;
    }

    // read number of points
    inputFile >> pointCount;

    // read initial velocity
    float vx0;
    float vy0;
    inputFile >> vx0 >> vy0;

    // read all points
    vector<float> x(pointCount, 0);
    vector<float> y(pointCount, 0);
    for (int i = 0; i < pointCount; i++) {
        // ignore index
     //   inputFile >> x[i];

        // read next coordinate
        inputFile >> x[i] >> y[i];
    }

    // read final velocity
    float vxf;
    float vyf;
    inputFile >> vxf >> vyf;

    // calculate the spline interpolation with the specified initial and final
    // velocity
    splineX.init(x, vx0, vxf);
    splineY.init(y, vy0, vyf);

    // calculate the a natural spline interpolation
    // splineX.init(x);
    // splineY.init(y);

    // output angle for each second
    std::ofstream outputFile(outPath);
    if (outputFile.is_open()) {
        for (int i = 0; i < pointCount; i++) {
            Vector3 r1, r2, r3;
            Vector3 position;
            getRotationAndPosition(i, 0, r1, r2, r3, position);
            float angle = atan2(r1.y(), r1.x());
            float degrees = radians_to_degrees(angle);
            outputFile << i << " n1:" << r1.x() << " n2:" << r1.y() << " angle:" << degrees << endl;
        }
    } else {
        std::cout << "ERROR at 1: Could not open file " << outPath << std::endl;
    }
}

void Problem2D::getRotationAndPosition(int startPointIndex, float time,
                                       Vector3& r1, Vector3& r2, Vector3& r3, Vector3& position) {
    // calculate forward direction
    float x = splineX.fn1(startPointIndex, time);
    float y = splineY.fn1(startPointIndex, time);
    r1.x() = x;
    r1.y() = y;
    r1.z() = 0;
    r1 = r1.normalized();

    // up direction is always constant
    Vector3 z(0, 0, 1);
    r3 = z.normalized();

    // side direction is the perpendicular to up and forward direction
    Vector3 c = z ^ r1;
    r2 = c.normalized();

    // set position
    position.x() = splineX.fn(startPointIndex, time);
    position.y() = splineY.fn(startPointIndex, time);
    position.z() = 0;
}
