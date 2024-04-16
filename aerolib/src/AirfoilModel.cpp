
#include "AirfoilModel.h"
#include "AirfoilGeometry.h"
#include "AirfoilElement.h"

void AirfoilModel::SetPlateShape() {
    ptShape.reserve(POINTCOUNT);
    // Implement setting plate shape here
}

void AirfoilModel::respline() {
    AirfoilElement e;
    e.setPoints((int)POINTCOUNT, ptShape);
    e.ReSpline((int)POINTCOUNT);
    ptShape.resize(POINTCOUNT);
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

