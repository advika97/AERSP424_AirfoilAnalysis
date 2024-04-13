#ifndef NACA4_H
#define NACA4_H

#include "AirfoilShape.h" // Include base class header
#include "DoubleParameter.h" // Include parameter class header
#include <cmath>
#include <string>
#include <vector>

class NACA4 : public AirfoilShape {
public:
    NACA4();
    NACA4(int PointCount, double Thickness, double ThicknessLocation, double Camber, double CamberLocation);
    virtual ~NACA4() = default;

    virtual std::vector<AirfoilParameter> getParameters();

private:
    void makeName(double dThick, double dXThick, double dCamber, double dXCamber);
};

#endif // NACA4_H
