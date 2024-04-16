#ifndef _AIRFOIL_H_
#define _AIRFOIL_H_

#include <string>
#include <cmath>
#include <locale>
#include <sstream>
#include <iomanip>
#include "AirfoilModel.h"

class Airfoil {
private:
    AirfoilModel* m_AirfoilModel;
    std::string m_BaseDir;
    static const int MAX_AIRFOIL = 14;
    double m_Cdfactor;
    double m_CdDelta;

public:
    Airfoil() : Airfoil(".") {}
    Airfoil(std::string BaseDir);
    virtual ~Airfoil();

    void setBaseDir(std::string BaseDir);

    bool Init(int nAirfoilNo) ;
    std::string getName() ;

    vector<DoublePoint> getShape();

    double getThickness();

    void setTEThickness(double TEThicknessPercent);

    double getAlfaStall(int Index);

    double getCl(double dAlfaDeg) ;

    double getCd(double dAlfaDeg);

    void setCdFactor(double CdFactor);

    void setCdDelta(double CdDelta);

    double getReynoldsNumber() ;

    double getMachNumber() ;
};

#endif
