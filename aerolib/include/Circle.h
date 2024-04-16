#ifndef CIRCLE_H
#define CIRCLE_H

#include "DoublePoint.h" // Assuming DoublePoint class is defined in DoublePoint.h

class Circle {
public:
    DoublePoint ptCenter;
    double radius;

    Circle();
    Circle(const DoublePoint& pt, double radius);
    Circle(const DoublePoint& pt1, const DoublePoint& pt2, const DoublePoint& pt3);
};

#endif // CIRCLE_H
