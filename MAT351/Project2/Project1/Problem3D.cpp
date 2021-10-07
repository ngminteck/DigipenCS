#include "Problem3D.h"

#include <fstream>
#include <iostream>

static const char* inPath = "sample3d.txt";
static const char* outPath = "output2.txt";

Problem3D::Problem3D() {
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
    float vz0;
    inputFile >> vx0 >> vy0 >> vz0;

    // read all points
    vector<float> x(pointCount, 0);
    vector<float> y(pointCount, 0);
    vector<float> z(pointCount, 0);
    for (int i = 0; i < pointCount; i++) {
        // ignore index
       // inputFile >> x[i];

        // read next coordinate
        inputFile >> x[i] >> y[i] >> z[i];
    }

    // read final velocity
    float vxf;
    float vyf;
    float vzf;
    inputFile >> vxf >> vyf >> vzf;

    // calculate the spline interpolation with the specified initial and final
    // velocity
    splineX.init(x, vx0, vxf);
    splineY.init(y, vy0, vyf);
    splineZ.init(z, vz0, vzf);

    // calculate the a natural spline interpolation
    // splineX = new Spline(x);
    // splineY = new Spline(y);
    // splineZ = new Spline(z);

    // output point number, normal vector and angle for each second
    std::ofstream outputFile(outPath);
    if (outputFile.is_open()) {
        for (int i = 0; i < pointCount; i++) {
            Vector3 t, n, b;
            Vector3 position;

            // calculate rotation matrix
            getRotationAndPosition(i, 0, t, n, b, position);
            Matrix r;
            r.setToRotation(t, n, b);

            // calculate axis-angle representation
            Vector3 axisAngle = r.calculateAxisAngle();
            float angle = axisAngle.w();

            // adjust angle to 0..360 degree and write to file
            if (angle < 0) angle += 2 * PI_F;
            outputFile << i << " n1:" << axisAngle.x() << " n2:" << axisAngle.y()
                       << " n3:" << axisAngle.z() << " angle:"
                       << radians_to_degrees(angle) << endl;
        }
    } else {
        std::cout << "ERROR at 1: Could not open file " << outPath << std::endl;
    }
}

void Problem3D::getRotationAndPosition(int startPointIndex, float time,
                                       Vector3& r1, Vector3& r2, Vector3& r3,
                                       Vector3& position) {
    // calculate direction
    float x1 = splineX.fn1(startPointIndex, time);
    float y1 = splineY.fn1(startPointIndex, time);
    float z1 = splineZ.fn1(startPointIndex, time);

    // the direction is the tangent vector for the Frenet-Serret formulas
    Vector3 tangent(x1, y1, z1);
    tangent = tangent.normalized();

    // calculate the binormal vector
    float x2 = splineX.fn2(startPointIndex, time);
    float y2 = splineY.fn2(startPointIndex, time);
    float z2 = splineZ.fn2(startPointIndex, time);
    Vector3 xv1(x1, y1, z1);
    Vector3 xv2(x2, y2, z2);
    xv1 = xv1.normalized();
    xv2 = xv2.normalized();
    Vector3 v = xv1 ^ xv2;
    Vector3 binormal = v.normalized();

    // calculate the normal vector
    Vector3 normal = binormal ^ tangent;
    normal = normal.normalized();

    // set rotation
    r1 = tangent;
    r2 = normal;
    r3 = binormal;

    // set position
    position.x() = splineX.fn(startPointIndex, time);
    position.y() = splineY.fn(startPointIndex, time);
    position.z() = splineZ.fn(startPointIndex, time);
}
