
#include "DoublePoint.h"
#include <cmath>
#include <string>


DoublePoint::DoublePoint() {
    dX = 0.0;
    dY = 0.0;
}

DoublePoint::DoublePoint(double X, double Y) {
    dX = X;
    dY = Y;
}

DoublePoint::DoublePoint(const DoublePoint& ptBase) {
    dX = ptBase.dX;
    dY = ptBase.dY;
}

void DoublePoint::subtract(double X, double Y) {
    dX -= X;
    dY -= Y;
}

void DoublePoint::subtract(const DoublePoint& ptSub) {
    dX -= ptSub.dX;
    dY -= ptSub.dY;
}

void DoublePoint::add(double X, double Y) {
    dX += X;
    dY += Y;
}

void DoublePoint::add(const DoublePoint& ptAdd) {
    dX += ptAdd.dX;
    dY += ptAdd.dY;
}

void DoublePoint::scale(const DoublePoint& ptMultiply) {
    dX *= ptMultiply.dX;
    dY *= ptMultiply.dY;
}

void DoublePoint::scale(double dFactor) {
    dX *= dFactor;
    dY *= dFactor;
}

void DoublePoint::max(double X, double Y) {
    dX = std::max(dX, X);
    dY = std::max(dY, Y);
}

void DoublePoint::max(const DoublePoint& ptMax) {
    dX = std::max(dX, ptMax.dX);
    dY = std::max(dY, ptMax.dY);
}

void DoublePoint::min(double X, double Y) {
    dX = std::min(dX, X);
    dY = std::min(dY, Y);
}

void DoublePoint::min(const DoublePoint& ptMin) {
    dX = std::min(dX, ptMin.dX);
    dY = std::min(dY, ptMin.dY);
}

double DoublePoint::getLength() {
    return std::hypot(dX, dY);
}

void DoublePoint::setLength(double Length) {
    Normalize();
    dX *= Length;
    dY *= Length;
}

void DoublePoint::Normalize() {
    double length = getLength();
    dX /= length;
    dY /= length;
}

void DoublePoint::arraycopy(DoublePoint* ptSrc, int nSrcOffset, DoublePoint* ptDst, int nDstOffset, int nLength) {
    for (int i = 0; i < nLength; i++) {
        ptDst[nDstOffset + i] = DoublePoint(ptSrc[nSrcOffset + i]);
    }
}

DoublePoint* DoublePoint::allocateArray(int nElements) {
    DoublePoint* aArray = new DoublePoint[nElements];
    for (int i = 0; i < nElements; i++) {
        aArray[i] = DoublePoint();
    }
    return aArray;
}

