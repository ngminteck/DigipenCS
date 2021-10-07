
#include "mathutil.h"

Vector3 Vector3::normalized() const {
    float l = length();
    return l > 0 ? ((*this) * (1.0f / l)) : (*this);
}

Vector3 Vector3::choosePerpendicular() const {
    // strategy: pick the two largest components,
    // permute them and negate one of them, and
    // replace the other (i.e. smallest) component with zero.

    float X = fabsf(x());
    float Y = fabsf(y());
    float Z = fabsf(z());

    Vector3 v;

    if (X < Y) {
        if (Y < Z) {
            // X < Y < Z
            v = Vector3(0, z(), -y());
        } else if (X < Z) {
            // X < Z <= Y
            v = Vector3(0, z(), -y());
        } else {
            // Z <= X < Y
            v = Vector3(y(), -x(), 0);
        }
    } else {
        if (Z < Y) {
            // Z < Y <= X
            v = Vector3(y(), -x(), 0);
        } else if (Z < X) {
            // Y <= Z < X
            v = Vector3(z(), 0, -x());
        } else {
            // Y <= X <= Z
            v = Vector3(z(), 0, -x());
        }
    }

#ifdef DEBUG
    float dotProduct = v * (*this);
    ASSERT_IS_EQUAL(dotProduct, 0);
#endif

    return v;
}

short Vector3::indexOfLeastComponent() const {
    float X = fabsf(x());
    float Y = fabsf(y());
    float Z = fabsf(z());
    if (Y <= Z) {
        return X <= Y ? 0 : 1;
    } else {
        return X <= Z ? 0 : 2;
    }
}

short Vector3::indexOfGreatestComponent() const {
    float X = fabsf(x());
    float Y = fabsf(y());
    float Z = fabsf(z());
    if (Y >= Z) {
        return X >= Y ? 0 : 1;
    } else {
        return X >= Z ? 0 : 2;
    }
}

float Vector3::computeAngle(const Vector3& v1, const Vector3& v2) {
    Vector3 cross = v1.normalized() ^ v2.normalized();

    // Due to numerical inaccuracies, the length of the cross product
    // may be slightly more than 1.
    // Calling arcsin on such a value would result in NaN.
    float lengthOfCross = cross.length();
    float angle = (lengthOfCross >= 1) ? PI_F / 2 : asinf(lengthOfCross);

    if (v1 * v2 < 0) angle = PI_F - angle;
    return angle;
}

float Vector3::computeSignedAngle(const Vector3& v1, const Vector3& v2,
                                  const Vector3& axis) {
    Vector3 cross = v1.normalized() ^ v2.normalized();

    // Due to numerical inaccuracies, the length of the cross product
    // may be slightly more than 1.
    // Calling arcsin on such a value would result in NaN.
    float lengthOfCross = cross.length();
    float angle = (lengthOfCross >= 1) ? PI_F / 2 : asinf(lengthOfCross);

    if (v1 * v2 < 0) angle = PI_F - angle;
    if (cross * axis < 0) angle = -angle;
    return angle;
}

void Matrix::setToIdentity() {
    _m[0] = 1;
    _m[4] = 0;
    _m[8] = 0;
    _m[12] = 0;
    _m[1] = 0;
    _m[5] = 1;
    _m[9] = 0;
    _m[13] = 0;
    _m[2] = 0;
    _m[6] = 0;
    _m[10] = 1;
    _m[14] = 0;
    _m[3] = 0;
    _m[7] = 0;
    _m[11] = 0;
    _m[15] = 1;
}

void Matrix::transpose() {
    float tmp;
#define SWAP(a, b) \
    (tmp) = (a);   \
    (a) = (b);     \
    (b) = (tmp);
    SWAP(_m[1], _m[4]);
    SWAP(_m[2], _m[8]);
    SWAP(_m[3], _m[12]);
    SWAP(_m[7], _m[13]);
    SWAP(_m[11], _m[14]);
    SWAP(_m[6], _m[9]);
#undef SWAP
}

void Matrix::setToTranslation(const Vector3& v) {
    _m[0] = 1;
    _m[4] = 0;
    _m[8] = 0;
    _m[12] = v.x();
    _m[1] = 0;
    _m[5] = 1;
    _m[9] = 0;
    _m[13] = v.y();
    _m[2] = 0;
    _m[6] = 0;
    _m[10] = 1;
    _m[14] = v.z();
    _m[3] = 0;
    _m[7] = 0;
    _m[11] = 0;
    _m[15] = 1;
}

void Matrix::setToScale(const Vector3& v) {
    _m[0] = v.x();
    _m[4] = 0;
    _m[8] = 0;
    _m[12] = 0;
    _m[1] = 0;
    _m[5] = v.y();
    _m[9] = 0;
    _m[13] = 0;
    _m[2] = 0;
    _m[6] = 0;
    _m[10] = v.z();
    _m[14] = 0;
    _m[3] = 0;
    _m[7] = 0;
    _m[11] = 0;
    _m[15] = 1;
}

void Matrix::setToScale(const Point3& origin, const Vector3& i,
                        const Vector3& j, const Vector3& k,
                        const Vector3& scaleFactor) {
    Matrix translationMatrix, rotationMatrix;
    translationMatrix.setToTranslation(-Vector3(origin));
    rotationMatrix.setToRotation(i, j, k);
    setToScale(scaleFactor);
    (*this) = (*this) * rotationMatrix * translationMatrix;
    rotationMatrix.transpose();  // equivalent to inverting the matrix
    translationMatrix.setToTranslation(Vector3(origin));
    (*this) = translationMatrix * rotationMatrix * (*this);
}

void Matrix::setToRotation(float angle_in_radians, const Vector3& v) {
    ASSERT_IS_NORMALIZED(v);
    float c = cosf(angle_in_radians);
    float s = sinf(angle_in_radians);
    float one_minus_c = 1 - c;
    _m[0] = c + one_minus_c * v.x() * v.x();
    _m[5] = c + one_minus_c * v.y() * v.y();
    _m[10] = c + one_minus_c * v.z() * v.z();
    _m[1] = _m[4] = one_minus_c * v.x() * v.y();
    _m[2] = _m[8] = one_minus_c * v.x() * v.z();
    _m[6] = _m[9] = one_minus_c * v.y() * v.z();
    float xs = v.x() * s;
    float ys = v.y() * s;
    float zs = v.z() * s;
    _m[1] += zs;
    _m[4] -= zs;
    _m[2] -= ys;
    _m[8] += ys;
    _m[6] += xs;
    _m[9] -= xs;

    _m[12] = 0;
    _m[13] = 0;
    _m[14] = 0;
    _m[3] = 0;
    _m[7] = 0;
    _m[11] = 0;
    _m[15] = 1;
}

void Matrix::setToRotation(float angle_in_radians, const Vector3& v,
                           const Point3& origin) {
    Matrix tmp;
    tmp.setToTranslation(-Vector3(origin));
    setToRotation(angle_in_radians, v);
    (*this) = (*this) * tmp;
    tmp.setToTranslation(Vector3(origin));
    (*this) = tmp * (*this);
}

void Matrix::setToRotation(const Vector3& v) {
    Matrix x;
    x.setToRotation(v.x(), Vector3(1, 0, 0));
    Matrix y;
    y.setToRotation(v.y(), Vector3(0, 1, 0));
    Matrix z;
    z.setToRotation(v.z(), Vector3(0, 0, 1));
    *this = (x * y * z);
}

void Matrix::setToRotation(const Vector3& i, const Vector3& j,
                           const Vector3& k) {
    _m[0] = i.x();
    _m[4] = i.y();
    _m[8] = i.z();
    _m[1] = j.x();
    _m[5] = j.y();
    _m[9] = j.z();
    _m[2] = k.x();
    _m[6] = k.y();
    _m[10] = k.z();

    _m[12] = 0;
    _m[13] = 0;
    _m[14] = 0;
    _m[3] = 0;
    _m[7] = 0;
    _m[11] = 0;
    _m[15] = 1;
}

void Matrix::setToLookAt(const Point3& eye, const Point3& target,
                         const Vector3& up, bool inverted) {
    // step one: generate a rotation matrix

    Vector3 z = (eye - target).normalized();
    Vector3 y = up;
    Vector3 x = y ^ z;
    y = z ^ x;

    // Cross product gives area of parallelogram, which is < 1 for
    // non-perpendicular unit-length vectors; so normalize x and y.
    x = x.normalized();
    y = y.normalized();

    Matrix m2;

    if (inverted) {
        // the rotation matrix
        _m[0] = x.x();
        _m[4] = y.x();
        _m[8] = z.x();
        _m[12] = 0;
        _m[1] = x.y();
        _m[5] = y.y();
        _m[9] = z.y();
        _m[13] = 0;
        _m[2] = x.z();
        _m[6] = y.z();
        _m[10] = z.z();
        _m[14] = 0;
        _m[3] = 0;
        _m[7] = 0;
        _m[11] = 0;
        _m[15] = 1;

        // step two: premultiply by a translation matrix
        m2.setToTranslation(Vector3(eye));
        *this = m2 * (*this);
    } else {
        // the rotation matrix
        _m[0] = x.x();
        _m[4] = x.y();
        _m[8] = x.z();
        _m[12] = 0;
        _m[1] = y.x();
        _m[5] = y.y();
        _m[9] = y.z();
        _m[13] = 0;
        _m[2] = z.x();
        _m[6] = z.y();
        _m[10] = z.z();
        _m[14] = 0;
        _m[3] = 0;
        _m[7] = 0;
        _m[11] = 0;
        _m[15] = 1;

        // step two: tack on a translation matrix
        m2.setToTranslation(-Vector3(eye));
        *this = (*this) * m2;
    }
}

void Matrix::setToFrustum(float left, float right, float bottom, float top,
                          float nearPlane, float farPlane, bool inverted) {
    if (inverted) {
        float one_over_2n = 0.5f / nearPlane;
        float one_over_2fn = one_over_2n / farPlane;

        // first row
        _m[0] = (right - left) * one_over_2n;
        _m[4] = 0;
        _m[8] = 0;
        _m[12] = (right + left) * one_over_2n;

        // second row
        _m[1] = 0;
        _m[5] = (top - bottom) * one_over_2n;
        _m[9] = 0;
        _m[13] = (top + bottom) * one_over_2n;

        // third row
        _m[2] = 0;
        _m[6] = 0;
        _m[10] = 0;
        _m[14] = -1;

        // fourth row
        _m[3] = 0;
        _m[7] = 0;
        _m[11] = (nearPlane - farPlane) * one_over_2fn;
        _m[15] = (nearPlane + farPlane) * one_over_2fn;
    } else {
        float two_n = 2 * nearPlane;
        float one_over_width = 1 / (right - left);
        float one_over_height = 1 / (top - bottom);
        float one_over_thickness = 1 / (farPlane - nearPlane);

        // first row
        _m[0] = two_n * one_over_width;
        _m[4] = 0;
        _m[8] = (right + left) * one_over_width;
        _m[12] = 0;

        // second row
        _m[1] = 0;
        _m[5] = two_n * one_over_height;
        _m[9] = (top + bottom) * one_over_height;
        _m[13] = 0;

        // third row
        _m[2] = 0;
        _m[6] = 0;
        _m[10] = -(farPlane + nearPlane) * one_over_thickness;
        _m[14] = -two_n * farPlane * one_over_thickness;

        // fourth row
        _m[3] = 0;
        _m[7] = 0;
        _m[11] = -1;
        _m[15] = 0;
    }
}

Vector3 Matrix::calculateAxisAngle() {
    float m00 = _m[0];
    float m01 = _m[1];
    float m02 = _m[2];
    float m10 = _m[4];
    float m11 = _m[5];
    float m12 = _m[6];
    float m20 = _m[8];
    float m21 = _m[9];
    float m22 = _m[10];

    float angle, x, y, z;  // variables for result
    float epsilon = 0.01f;  // margin to allow for rounding errors
    float epsilon2 = 0.1f;  // margin to distinguish between 0 and 180 degrees
    // optional check that input is pure rotation, 'isRotationMatrix' is defined
    // at:
    // https://www.euclideanspace.com/maths/algebra/matrix/orthogonal/rotation/
    if ((fabsf(m01 - m10) < epsilon) &&
        (fabsf(m02 - m20) < epsilon) &&
        (fabsf(m12 - m21) < epsilon)) {
        // singularity found
        // first check for identity matrix which must have +1 for all terms
        //  in leading diagonaland zero in other terms
        if ((fabsf(m01 + m10) < epsilon2) &&
            (fabsf(m02 + m20) < epsilon2) &&
            (fabsf(m12 + m21) < epsilon2) &&
            (fabsf(m00 + m11 + m22 - 3) < epsilon2)) {
            // this singularity is identity matrix so angle = 0
            return Vector3(1, 0, 0, 0);  // zero angle, arbitrary axis
        }
        // otherwise this singularity is angle = 180
        angle = PI_F;
        float xx = (m00 + 1) / 2;
        float yy = (m11 + 1) / 2;
        float zz = (m22 + 1) / 2;
        float xy = (m01 + m10) / 4;
        float xz = (m02 + m20) / 4;
        float yz = (m12 + m21) / 4;
        if ((xx > yy) && (xx > zz)) {  // m00 is the largest diagonal term
            if (xx < epsilon) {
                x = 0;
                y = 0.7071f;
                z = 0.7071f;
            } else {
                x = sqrtf(xx);
                y = xy / x;
                z = xz / x;
            }
        } else if (yy > zz) {  // m11 is the largest diagonal term
            if (yy < epsilon) {
                x = 0.7071f;
                y = 0;
                z = 0.7071f;
            } else {
                y = sqrtf(yy);
                x = xy / y;
                z = yz / y;
            }
        } else {  // m22 is the largest diagonal term so base result on this
            if (zz < epsilon) {
                x = 0.7071f;
                y = 0.7071f;
                z = 0;
            } else {
                z = sqrtf(zz);
                x = xz / z;
                y = yz / z;
            }
        }
        return Vector3(x, y, z, angle);  // return 180 deg rotation
    }
    // as we have reached here there are no singularities so we can handle
    // normally
    float s =
        sqrtf((m21 - m12) * (m21 - m12) +
              (m02 - m20) * (m02 - m20) +
              (m10 - m01) * (m10 - m01));  // used to normalise
    if (fabsf(s) < 0.001) s = 1;
    // prevent divide by zero, should not happen if matrix is orthogonal and
    // should be caught by singularity test above, but I've left it in just in
    // case
    angle = acosf((m00 + m11 + m22 - 1) / 2);
    x = (m21 - m12) / s;
    y = (m02 - m20) / s;
    z = (m10 - m01) / s;
    return Vector3(x, y, z, angle);
}

void AlignedBox::bound(const Point3& p) {
    if (_isEmpty) {
        _p0 = _p1 = p;
        _isEmpty = false;
    } else {
        if (p.x() < _p0.x())
            _p0.x() = p.x();
        else if (p.x() > _p1.x())
            _p1.x() = p.x();

        if (p.y() < _p0.y())
            _p0.y() = p.y();
        else if (p.y() > _p1.y())
            _p1.y() = p.y();

        if (p.z() < _p0.z())
            _p0.z() = p.z();
        else if (p.z() > _p1.z())
            _p1.z() = p.z();
    }
}

void AlignedBox::bound(const vector<Point3>& l, const Matrix& m) {
    // The naive way to bound a set of points is to loop through
    // them one-by-one, calling bound() on each point.
    // That would require up to 6 comparisons for each point.
    // Below, we use a slightly smarter method that requires
    // 9 comparisons per *pair* of points, hence it's 25 % less work.

    vector<Point3>::const_iterator it = l.begin();
    if (l.size() < 3) {
        for (; it != l.end(); ++it) {
            bound(m * (*it));
        }
    } else {
        // If we have lots of points, it's efficient to gather
        // up the min and max for each component, handling *two*
        // points at a time.

        // To avoid having to deal a special case within the loop,
        // we must ensure that the bounding box is not empty
        // (possibly by processing an initial point).
        // To ensure the loop can process the points in pairs,
        // we must also ensure that there is an even number of
        // points left to process (again, possibly by processing
        // an initial point).
        if (l.size() % 2 == 0) {
            if (_isEmpty) {
                bound(m * (*it));
                ++it;
                bound(m * (*it));
                ++it;
            }
        } else {
            bound(m * (*it));
            ++it;
        }

        ASSERT(!_isEmpty);

        // Now we loop through the remaining points, processing two at a time.
        Point3 a, b;
        while (it != l.end()) {
            a = m * (*it);
            ++it;
            ASSERT(it != l.end());
            b = m * (*it);
            ++it;

            if (a.x() < b.x()) {
                if (a.x() < _p0.x()) _p0.x() = a.x();
                if (b.x() > _p1.x()) _p1.x() = b.x();
            } else {
                if (b.x() < _p0.x()) _p0.x() = b.x();
                if (a.x() > _p1.x()) _p1.x() = a.x();
            }

            if (a.y() < b.y()) {
                if (a.y() < _p0.y()) _p0.y() = a.y();
                if (b.y() > _p1.y()) _p1.y() = b.y();
            } else {
                if (b.y() < _p0.y()) _p0.y() = b.y();
                if (a.y() > _p1.y()) _p1.y() = a.y();
            }

            if (a.z() < b.z()) {
                if (a.z() < _p0.z()) _p0.z() = a.z();
                if (b.z() > _p1.z()) _p1.z() = b.z();
            } else {
                if (b.z() < _p0.z()) _p0.z() = b.z();
                if (a.z() > _p1.z()) _p1.z() = a.z();
            }
        }
    }
}

void AlignedBox::bound(const AlignedBox& b) {
    if (!b.isEmpty()) {
        ASSERT(b._p0.x() <= b._p1.x());
        ASSERT(b._p0.y() <= b._p1.y());
        ASSERT(b._p0.z() <= b._p1.z());
        if (_isEmpty) {
            _p0 = b._p0;
            _p1 = b._p1;
            _isEmpty = false;
        } else {
            if (b._p0.x() < _p0.x()) _p0.x() = b._p0.x();
            if (b._p1.x() > _p1.x()) _p1.x() = b._p1.x();

            if (b._p0.y() < _p0.y()) _p0.y() = b._p0.y();
            if (b._p1.y() > _p1.y()) _p1.y() = b._p1.y();

            if (b._p0.z() < _p0.z()) _p0.z() = b._p0.z();
            if (b._p1.z() > _p1.z()) _p1.z() = b._p1.z();
        }
    }
}

bool AlignedBox::intersects(const Ray& ray) const {
    // We compute a bounding sphere for the box.
    // If the ray intersects the bounding sphere,
    // it *may* intersect the box.
    Point3 intersection;
    return Sphere(getCentre(), (_p1 - _p0).length() / 2)
        .intersects(ray, intersection);
}

bool AlignedBox::intersectsExactly(const Ray& ray, Point3& intersection) {
    bool intersectionDetected = false;
    float distance = 0;

    // candidate intersection
    float candidateDistance;
    Point3 candidatePoint;

    if (ray.direction.x() != 0) {
        candidateDistance = -(ray.origin.x() - _p0.x()) / ray.direction.x();
        if (distance >= 0 &&
            (!intersectionDetected || candidateDistance < distance)) {
            candidatePoint = ray.origin + ray.direction * candidateDistance;
            if (_p0.y() <= candidatePoint.y() &&
                candidatePoint.y() <= _p1.y() &&
                _p0.z() <= candidatePoint.z() &&
                candidatePoint.z() <= _p1.z()) {
                distance = candidateDistance;
                intersection = candidatePoint;
                intersectionDetected = true;
            }
        }
        candidateDistance = -(ray.origin.x() - _p1.x()) / ray.direction.x();
        if (distance >= 0 &&
            (!intersectionDetected || candidateDistance < distance)) {
            candidatePoint = ray.origin + ray.direction * candidateDistance;
            if (_p0.y() <= candidatePoint.y() &&
                candidatePoint.y() <= _p1.y() &&
                _p0.z() <= candidatePoint.z() &&
                candidatePoint.z() <= _p1.z()) {
                distance = candidateDistance;
                intersection = candidatePoint;
                intersectionDetected = true;
            }
        }
    }
    if (ray.direction.y() != 0) {
        candidateDistance = -(ray.origin.y() - _p0.y()) / ray.direction.y();
        if (distance >= 0 &&
            (!intersectionDetected || candidateDistance < distance)) {
            candidatePoint = ray.origin + ray.direction * candidateDistance;
            if (_p0.x() <= candidatePoint.x() &&
                candidatePoint.x() <= _p1.x() &&
                _p0.z() <= candidatePoint.z() &&
                candidatePoint.z() <= _p1.z()) {
                distance = candidateDistance;
                intersection = candidatePoint;
                intersectionDetected = true;
            }
        }
        candidateDistance = -(ray.origin.y() - _p1.y()) / ray.direction.y();
        if (distance >= 0 &&
            (!intersectionDetected || candidateDistance < distance)) {
            candidatePoint = ray.origin + ray.direction * candidateDistance;
            if (_p0.x() <= candidatePoint.x() &&
                candidatePoint.x() <= _p1.x() &&
                _p0.z() <= candidatePoint.z() &&
                candidatePoint.z() <= _p1.z()) {
                distance = candidateDistance;
                intersection = candidatePoint;
                intersectionDetected = true;
            }
        }
    }
    if (ray.direction.z() != 0) {
        candidateDistance = -(ray.origin.z() - _p0.z()) / ray.direction.z();
        if (distance >= 0 &&
            (!intersectionDetected || candidateDistance < distance)) {
            candidatePoint = ray.origin + ray.direction * candidateDistance;
            if (_p0.y() <= candidatePoint.y() &&
                candidatePoint.y() <= _p1.y() &&
                _p0.x() <= candidatePoint.x() &&
                candidatePoint.x() <= _p1.x()) {
                distance = candidateDistance;
                intersection = candidatePoint;
                intersectionDetected = true;
            }
        }
        candidateDistance = -(ray.origin.z() - _p1.z()) / ray.direction.z();
        if (distance >= 0 &&
            (!intersectionDetected || candidateDistance < distance)) {
            candidatePoint = ray.origin + ray.direction * candidateDistance;
            if (_p0.y() <= candidatePoint.y() &&
                candidatePoint.y() <= _p1.y() &&
                _p0.x() <= candidatePoint.x() &&
                candidatePoint.x() <= _p1.x()) {
                distance = candidateDistance;
                intersection = candidatePoint;
                intersectionDetected = true;
            }
        }
    }
    return intersectionDetected;
}

bool Plane::intersects(const Ray& ray, Point3& intersection,
                       bool allowIntersectionEvenIfPlaneIsBackfacing) {
    float dot = _n * ray.direction;
    if (!allowIntersectionEvenIfPlaneIsBackfacing && dot > 0) {
        return false;
    }
    if (dot == 0) {
        return false;
    }

    // See Foley and van Dam, pg 1101
    Point3 pointOnPlane(_n * (-_d));
    float t = (pointOnPlane - ray.origin) * _n / dot;

    if (t < 0) {
        return false;
    }

    intersection = ray.point(t);
    return true;
}

Quaternion Quaternion::EulerToQuaternion(Vector3 euler) {
    float c1 = cosf(euler.z() * 0.5f);
    float c2 = cosf(euler.y() * 0.5f);
    float c3 = cosf(euler.x() * 0.5f);
    float s1 = sinf(euler.z() * 0.5f);
    float s2 = sinf(euler.y() * 0.5f);
    float s3 = sinf(euler.x() * 0.5f);

    Quaternion q;
    q.x() = c1 * c2 * s3 - s1 * s2 * c3;
    q.y() = c1 * s2 * c3 + s1 * c2 * s3;
    q.z() = s1 * c2 * c3 - c1 * s2 * s3;
    q.w() = c1 * c2 * c3 + s1 * s2 * s3;

    return q;
}

Vector3 Quaternion::toEuler() const {
    Vector3 euler;
    const static float PI_OVER_2 = PI_F * 0.5f;
    const static float EPSILON = 1e-10f;
    float sqw, sqx, sqy, sqz;

    // quick conversion to Euler angles to give tilt to user
    sqw = (*this).w() * (*this).w();
    sqx = (*this).x() * (*this).x();
    sqy = (*this).y() * (*this).y();
    sqz = (*this).z() * (*this).z();

    euler.y() =
        asinf(2.0f * ((*this).w() * (*this).y() - (*this).x() * (*this).z()));
    if (PI_OVER_2 - fabsf(euler.y()) > EPSILON) {
        euler.z() = atan2f(
            2.0f * ((*this).x() * (*this).y() + (*this).w() * (*this).z()),
            sqx - sqy - sqz + sqw);
        euler.x() = atan2f(
            2.0f * ((*this).w() * (*this).x() + (*this).y() * (*this).z()),
            sqw - sqx - sqy + sqz);
    } else {
        // compute heading from local 'down' vector
        euler.z() = atan2f(
            2 * (*this).y() * (*this).z() - 2 * (*this).x() * (*this).w(),
            2 * (*this).x() * (*this).z() + 2 * (*this).y() * (*this).w());
        euler.x() = 0.0f;

        // If facing down, reverse yaw
        if (euler.y() < 0) euler.z() = PI_F - euler.z();
    }
    return euler;
}

bool Sphere::intersects(const Ray& ray, Point3& intersection,
                        bool allowIntersectionEvenIfRayOriginatesInsideSphere) {
    // a line of the form Q+v*t, where t is real
    const Point3& Q = ray.origin;
    const Vector3& v = ray.direction;

    // the sphere's centre
    const Point3& P = _centre;

    ASSERT_IS_NORMALIZED(v);
    // see Foley and van Dam, pg 1101
    Vector3 Q_minus_P(Q - P);
    float b = 2 * (v * Q_minus_P);
    float c = Q_minus_P.lengthSquared() - _radiusSquared;

    // Consider the quadratic equation
    //   t^2 + b*t + c = 0
    // If there are real roots, then the line intersects the sphere.
    // If there are *positive* roots, the the *ray* intersects the sphere.
    float determinant = b * b - 4 * c;
    if (determinant >= 0) {
        // see Numerical Recipes in C, pg 184
        float q = -0.5f * (b + (b > 0 ? 1 : -1) * sqrtf(determinant));
        float t1 = q;
        float t2 = c / q;
        if (t1 >= 0 && t2 >= 0) {
            intersection = ray.point(t1 < t2 ? t1 : t2);
            return true;
        } else {
            // At least one of the intersection points has a negative t value.
            // This implies that either there's no intersection between the
            // ray and sphere, or the origin of the ray is inside the sphere.
            if (allowIntersectionEvenIfRayOriginatesInsideSphere) {
                if (t1 >= 0) {
                    intersection = ray.point(t1);
                    return true;
                } else if (t2 >= 0) {
                    intersection = ray.point(t2);
                    return true;
                }
            }
        }
    }
    return false;
}

Vector3 operator*(const Matrix& m, const Vector3& v) {
    return Vector3(m(0) * v.x() + m(4) * v.y() + m(8) * v.z() + m(12) * v.w(),
                   m(1) * v.x() + m(5) * v.y() + m(9) * v.z() + m(13) * v.w(),
                   m(2) * v.x() + m(6) * v.y() + m(10) * v.z() + m(14) * v.w(),
                   m(3) * v.x() + m(7) * v.y() + m(11) * v.z() + m(15) * v.w());
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    Matrix m;

    m(0) = a(0) * b(0) + a(4) * b(1) + a(8) * b(2) + a(12) * b(3);
    m(1) = a(1) * b(0) + a(5) * b(1) + a(9) * b(2) + a(13) * b(3);
    m(2) = a(2) * b(0) + a(6) * b(1) + a(10) * b(2) + a(14) * b(3);
    m(3) = a(3) * b(0) + a(7) * b(1) + a(11) * b(2) + a(15) * b(3);

    m(4) = a(0) * b(4) + a(4) * b(5) + a(8) * b(6) + a(12) * b(7);
    m(5) = a(1) * b(4) + a(5) * b(5) + a(9) * b(6) + a(13) * b(7);
    m(6) = a(2) * b(4) + a(6) * b(5) + a(10) * b(6) + a(14) * b(7);
    m(7) = a(3) * b(4) + a(7) * b(5) + a(11) * b(6) + a(15) * b(7);

    m(8) = a(0) * b(8) + a(4) * b(9) + a(8) * b(10) + a(12) * b(11);
    m(9) = a(1) * b(8) + a(5) * b(9) + a(9) * b(10) + a(13) * b(11);
    m(10) = a(2) * b(8) + a(6) * b(9) + a(10) * b(10) + a(14) * b(11);
    m(11) = a(3) * b(8) + a(7) * b(9) + a(11) * b(10) + a(15) * b(11);

    m(12) = a(0) * b(12) + a(4) * b(13) + a(8) * b(14) + a(12) * b(15);
    m(13) = a(1) * b(12) + a(5) * b(13) + a(9) * b(14) + a(13) * b(15);
    m(14) = a(2) * b(12) + a(6) * b(13) + a(10) * b(14) + a(14) * b(15);
    m(15) = a(3) * b(12) + a(7) * b(13) + a(11) * b(14) + a(15) * b(15);

    return m;
}

float LineSegment::distanceToPointSquared(int P_x, int P_y) const {
    /*
       Consider the line B+t(A-B) through points A and B, where t is real.
       Let R be the point on the line closest to a given point P.
       We have

          (R-P)*(A-B) = 0

       Substituting in R=B+t(A-B),

          ( t(A-B) + B-P )*(A-B) = 0

          t(A-B)*(A-B) + (B-P)*(A-B) = 0

          t = (P-B)*(A-B) / |A-B|^2

       If 0 <= t <= 1, then R lies between A and B.
       Equivalently, if 0 <= (P-B)*(A-B) <= |A-B|^2,
       then R lies between A and B.

       Now, the distance squared between P and the line is

          |P-R|^2 = (P-R)*(P-R)
                  = ( P-B - t(A-B) )*( P-B - t(A-B) )
                  = (P-B)*(P-B) + t^2(A-B)*(A-B) - 2t(P-B)*(A-B)
                  = |P-B|^2 + t^2|A-B|^2 - 2t(P-B)*(A-B)

       But (P-B)*(A-B) = t|A-B|^2.  Substituting in,

          |P-R|^2 = |P-B|^2 + t(P-B)*(A-B) - 2t(P-B)*(A-B)
                  = |P-B|^2 - t(P-B)*(A-B)

       and t = (P-B)*(A-B) / |A-B|^2, so

          |P-R|^2 = |P-B|^2 - ((P-B)*(A-B))^2/|A-B|^2
    */
    int A_minus_B_x = A_x - B_x;
    int A_minus_B_y = A_y - B_y;
    float A_minus_B_length2 =
        float(A_minus_B_x * A_minus_B_x + A_minus_B_y * A_minus_B_y);
    int P_minus_B_x = P_x - B_x;
    int P_minus_B_y = P_y - B_y;
    float dot = float(A_minus_B_x * P_minus_B_x + A_minus_B_y * P_minus_B_y);
    float distance;
    if (dot <= 0) {
        // return the distance squared from P to B
        distance = float(P_minus_B_x * P_minus_B_x + P_minus_B_y * P_minus_B_y);
    } else if (dot >= A_minus_B_length2) {
        // return the distance squared from P to A
        int P_minus_A_x = P_x - A_x;
        int P_minus_A_y = P_y - A_y;
        distance = float(P_minus_A_x * P_minus_A_x + P_minus_A_y * P_minus_A_y);
    } else {
        ASSERT(A_minus_B_length2 > 0);
        // return the distance squared from P to the line

#if 0  // This is correct, but ...
      distance = P_minus_B_x * P_minus_B_x + P_minus_B_y * P_minus_B_y
         - dot*dot/A_minus_B_length2;
#else  // ... this is slightly more efficient (and yet equivalent).
        float numerator =
            float(P_minus_B_y * A_minus_B_x - P_minus_B_x * A_minus_B_y);
        distance = numerator * numerator / A_minus_B_length2;
#endif
    }
    return distance;
}

float LineSegment::scaledComponent(int dx, int dy) const {
    if (dx == 0 && dy == 0) return 0;

    // interpret the line segment as a vector
    int dx2 = B_x - A_x;
    int dy2 = B_y - A_y;
    if (dx2 == 0 && dy2 == 0) return 0;

    // compute the dot product of the vectors
    float d = float(dx * dx2 + dy * dy2);

    // divide by the length squared of the line segment
    float lengthSquared2 = float(dx2 * dx2 + dy2 * dy2);
    return d / lengthSquared2;
}

float degrees_to_radians(float degrees) { return degrees * (PI_F / 180.0f); }

float radians_to_degrees(float radians) { return 180.0f * radians / PI_F; }

Vector3 axisRotToEuler(float radian, Vector3 axis) {
    axis = axis.normalized();

    float x = axis.x(), y = axis.y(), z = axis.z();
    float heading, attitude, bank;

    float s = sinf(radian);
    float c = cosf(radian);
    float t = 1 - c;

    if ((x * y * t + z * s) > 0.998f) {  // north pole singularity detected
        heading = 2 * atan2f(x * sinf(radian / 2), cosf(radian / 2));
        attitude = PI_F / 2;
        bank = 0;
        return Vector3(attitude, heading, bank);
    }
    if ((x * y * t + z * s) < -0.998f) {  // south pole singularity detected
        heading = -2 * atan2f(x * sinf(radian / 2), cosf(radian / 2));
        attitude = -PI_F / 2;
        bank = 0;
        return Vector3(attitude, heading, bank);
    }

    heading = atan2f(y * s - x * z * t, 1 - (y * y + z * z) * t);
    attitude = asinf(x * y * t + z * s);
    bank = atan2f(x * s - y * z * t, 1 - (x * x + z * z) * t);

    return Vector3(attitude, heading, bank);
}

Quaternion Slerp(Quaternion q1, Quaternion q2, float t) {
    float dotR = Quaternion::Dot(q1, q2);
    // for Quaternion Slerp we have to ceil it to the acos range because
    // precision messes up in some cases
    float comp = dotR >= 0.0f ? 1.0f : -1.0f;
    float cosom = dotR * comp;

    float coeff1, coeff2;
    if (cosom < 0.9999f) {
        float theta = acosf(cosom);
        float deno = 1.0f / sinf(theta);
        coeff1 = sinf((1.0f - t) * theta) * deno;
        coeff2 = sinf(t * theta) * deno;
    } else {
        coeff1 = 1.0f - t;
        coeff2 = t;
    }

    coeff1 *= comp;

    return Quaternion((q1.x() * coeff1) + (q2.x() * coeff2),
                      (q1.y() * coeff1) + (q2.y() * coeff2),
                      (q1.z() * coeff1) + (q2.z() * coeff2),
                      (q1.w() * coeff1) + (q2.w() * coeff2));
}
