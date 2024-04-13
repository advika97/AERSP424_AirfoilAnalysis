#include <string>
#include "DoublePoint.h"

class AirfoilModel {
public:
    std::string sName = "unknown";
    DoublePoint* ptShape = nullptr;
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
    void setShapeFromFile(std::string filePath);
    void respline();
    virtual void setTEThickness(double TEThicknessPercent) = 0;
};


