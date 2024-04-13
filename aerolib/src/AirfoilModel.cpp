
#include "AirfoilModel.h"
#include "AirfoilGeometry.h"
#include "AirfoilElement.h"

void AirfoilModel::SetPlateShape() {
    ptShape = DoublePoint::allocateArray(POINTCOUNT);
    // Implement setting plate shape here
}

void AirfoilModel::setShapeFromFile(std::string filePath) {
    AirfoilGeometry theShape;
    if (theShape.fromFile(filePath)) {
        AirfoilElement& e = theShape.Element[0];
        ptShape = DoublePoint::allocateArray(e.PointCount);
        e.getPoints(ptShape);
        respline();
    }
}

void AirfoilModel::respline() {
    AirfoilElement e;
    e.setPoints(POINTCOUNT, ptShape);
    e.ReSpline(POINTCOUNT);
    ptShape = DoublePoint::allocateArray(POINTCOUNT);
    e.getPoints(ptShape);
}

void AirfoilModel::setTEThickness(double TEThickness) {
    AirfoilElement e;
    e.setPoints(POINTCOUNT, ptShape);
    e.setTEThickness(TEThickness);
    e.getPoints(ptShape);
}

double AirfoilModel::getReynoldsNumber() {
    return ReynoldsNumber;
}

double AirfoilModel::getMachNumber() {
    return MachNumber;
}

