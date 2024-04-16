#include "Circle.h"
#include <cmath>

Circle::Circle() : ptCenter(DoublePoint()), radius(0.0) {}

Circle::Circle(const DoublePoint& pt, double radius) : ptCenter(pt), radius(radius) {}

Circle::Circle(const DoublePoint& pt1, const DoublePoint& pt2, const DoublePoint& pt3) : radius(0.0) {
    double dx12 = pt1.dX - pt2.dX;
    double dx23 = pt2.dX - pt3.dX;
    double dx13 = pt1.dX - pt3.dX;
    double dy12 = pt1.dY - pt2.dY;
    double dy23 = pt2.dY - pt3.dY;
    double dy13 = pt1.dY - pt3.dY;
    double x1_2 = pow(pt1.dX, 2.0);
    double x2_2 = pow(pt2.dX, 2.0);
    double x3_2 = pow(pt3.dX, 2.0);
    double y1_2 = pow(pt1.dY, 2.0);
    double y2_2 = pow(pt2.dY, 2.0);
    double y3_2 = pow(pt3.dY, 2.0);

    double divi = 2.0 * (pt1.dX * dy23 - pt2.dX * dy13 + pt3.dX * dy12);

    if (std::abs(divi) < 1.0E-16) {
        if (divi == 0.0) {
            divi = 1.0E-16;
        }
        else {
            divi = std::signbit(divi) ? -1.0E-16 : 1.0E-16;
        }
    }

    ptCenter.dX = ((x1_2 + y1_2) * dy23 - (x2_2 + y2_2) * dy13 + (x3_2 + y3_2) * dy12) / divi;
    ptCenter.dY = -((y1_2 + x1_2) * dx23 - (y2_2 + x2_2) * dx13 + (y3_2 + x3_2) * dx12) / divi;
    radius = sqrt(pow(pt1.dX - ptCenter.dX, 2.0) + pow(pt1.dY - ptCenter.dY, 2.0));
}
