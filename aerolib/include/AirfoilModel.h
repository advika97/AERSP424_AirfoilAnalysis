#ifndef _AIRFOILMODEL_H_
#define _AIRFOILMODEL_H_

#include <string>
#include <vector>
#include "DoublePoint.h"

using namespace std;

class AirfoilModel {
public:
    std::string sName = "unknown";
    vector<DoublePoint> ptShape;
    static const int POINTCOUNT = 121;
    double dAlfaStall[2];
    double ReynoldsNumber = 0.0;
    double MachNumber = 0.0;

    virtual bool Init(int nAirfoilIndex) = 0;
    virtual double getCl(double dAlfaDeg) = 0;
    virtual double getCd(double dAlfaDeg) = 0;
    virtual double getReynoldsNumber() = 0;
    virtual double getMachNumber() = 0;
    void SetPlateShape();
    void respline();
    virtual void setTEThickness(double TEThicknessPercent) = 0;
};


#endif
