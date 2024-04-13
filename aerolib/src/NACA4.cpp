#include "NACA4.h"

NACA4::NACA4() {
    m_TypeName = "P_NACA_4";
    m_Comment = "C_NACA4";
    //m_parameters = nullptr;
}

NACA4::NACA4(int PointCount, double Thickness, double ThicknessLocation, double Camber, double CamberLocation) : NACA4() {
    PointCount = PointCount / 2 + 1 + PointCount / 2;
    allocatePoints(PointCount);

    Camber = std::max(1.0E-10, Camber);
    CamberLocation = std::max(1.0E-4, CamberLocation);
    CamberLocation = std::min(0.9999, CamberLocation);
    ThicknessLocation = std::max(1.0E-4, ThicknessLocation);
    ThicknessLocation = std::min(0.9999, ThicknessLocation);
    int nSteps = (PointCount - 1) / 2;

    for (int nCount = 0; nCount < (PointCount + 1) / 2; ++nCount) {
        double dPhi = static_cast<double>(nCount) * 3.141592653589793 / static_cast<double>(nSteps);
        double dX = 0.5 * (1.0 + cos(dPhi));
        double dYc, dSt;

        if (dX < CamberLocation) {
            dYc = Camber * (2.0 * CamberLocation * dX - dX * dX) / (CamberLocation * CamberLocation);
            dSt = 2.0 * Camber * (CamberLocation - dX) / (CamberLocation * CamberLocation);
        } else {
            dYc = Camber * (1.0 - 2.0 * CamberLocation + 2.0 * CamberLocation * dX - dX * dX) / pow(1.0 - CamberLocation, 2.0);
            dSt = 2.0 * Camber * (CamberLocation - dX) / pow(1.0 - CamberLocation, 2.0);
        }

        double dYt = Thickness * (0.2969 * sqrt(dX) - 0.126 * dX - 0.3516 * dX * dX + 0.2843 * pow(dX, 3.0) - 0.1015 * pow(dX, 4.0)) / 0.2;
        double dSn = dSt / sqrt(1.0 + dSt * dSt);
        double dCs = 1.0 / sqrt(1.0 + dSt * dSt);

        m_X[nCount] = dX - dYt * dSn;
        m_X[PointCount - 1 - nCount] = dX + dYt * dSn;
        m_Y[nCount] = dYc + dYt * dCs;
        m_Y[PointCount - 1 - nCount] = dYc - dYt * dCs;
    }

    makeName(Thickness, ThicknessLocation, Camber, CamberLocation);
}

void NACA4::makeName(double dThick, double dXThick, double dCamber, double dXCamber) {

    std::string m_sThickness = std::to_string(static_cast<long>((int)(100.0 * dThick)));
    if (dThick < 0.1) {
        m_sThickness = "0" + m_sThickness;
    }

    std::string m_sCamberLocation = std::to_string(static_cast<long>((int)(10.0 * dXCamber)));
    if (std::abs(dCamber) < 0.001) {
        m_sCamberLocation = "0";
    }

    m_sName = "NACA " + std::to_string(static_cast<long>((int)(100.0 * dCamber))) + m_sCamberLocation + m_sThickness;
    if (std::abs(dXThick - 0.3) > 0.001) {
        m_sName = m_sName + "-6" + std::to_string(static_cast<long>((int)(10.0 * dXThick)));
    }
}

std::vector<AirfoilParameter> NACA4::getParameters() {
    m_parameters.push_back(DoubleParameter("THK", "%", 10.0, 0.01, 99.9, 0.5));
    m_parameters.push_back(DoubleParameter("CAM", "%", 2.0, -99.9, 99.9, 1.0));
    m_parameters.push_back(DoubleParameter("XCAM", "%", 40.0, 0.01, 99.9, 1.0));
    return m_parameters;
}
