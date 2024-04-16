
#include "Airfoil.h"
#include "TabulatedAirfoil.h"
#include "AnalyticalAirfoil.h"

Airfoil::Airfoil(std::string BaseDir) {
    m_Cdfactor = 1.0;
    m_CdDelta = 0.0;
    m_AirfoilModel = nullptr;
    m_BaseDir = BaseDir;
}

Airfoil::~Airfoil()
{
    if (m_AirfoilModel != nullptr)
        delete m_AirfoilModel;
}

void Airfoil::setBaseDir(std::string BaseDir) {
    m_BaseDir = BaseDir;
}

bool Airfoil::Init(int nAirfoilNo) {
    bool bRetVal = false;
    try {
        int nAirfoilIndex;
        if (nAirfoilNo > 14) {
            m_AirfoilModel = new TabulatedAirfoil(m_BaseDir);
            nAirfoilIndex = nAirfoilNo - 14;
        } else {
            m_AirfoilModel = new AnalyticalAirfoil();
            nAirfoilIndex = nAirfoilNo;
        }

        if (m_AirfoilModel != nullptr) {
            bRetVal = m_AirfoilModel->Init(nAirfoilIndex);
        }
    } catch (const std::exception&) {
    }
    return bRetVal;
}

std::string Airfoil::getName() {
    std::stringstream name;
    name << m_AirfoilModel->sName;
    std::locale loc;
    std::stringstream theFormat;
    theFormat.imbue(loc);
    theFormat << std::fixed << std::setprecision(1);
    double ReNo = m_AirfoilModel->getReynoldsNumber();
    if (ReNo > 0.0) {
        name << ", Re=";
        if (ReNo > 1000000.0) {
            name << theFormat.str() << (ReNo / 1000000.0) << "x10^6";
        } else {
            theFormat.str("");
            theFormat << std::fixed << std::setprecision(0);
            name << theFormat.str() << ReNo;
        }
    }

    double MachNo = m_AirfoilModel->getMachNumber();
    if (MachNo > 0.0) {
        theFormat.str("");
        theFormat << std::fixed << std::setprecision(2);
        name << ", M=" << theFormat.str() << MachNo;
    }

    return name.str();
}

vector<DoublePoint> Airfoil::getShape() {
    return m_AirfoilModel->ptShape;
}

double Airfoil::getThickness() {
    double yMax = -1000.0;
    double yMin = 1000.0;
    if (m_AirfoilModel->ptShape.size() > 0) {
        for (int i = 0; i < sizeof(m_AirfoilModel->ptShape) / sizeof(DoublePoint); ++i) {
            yMax = std::max(yMax, m_AirfoilModel->ptShape[i].dY);
            yMin = std::min(yMin, m_AirfoilModel->ptShape[i].dY);
        }
    }
    return yMax - yMin;
}

void Airfoil::setTEThickness(double TEThicknessPercent) {
    m_AirfoilModel->setTEThickness(TEThicknessPercent);
}

double Airfoil::getAlfaStall(int Index) {
    return m_AirfoilModel->dAlfaStall[Index];
}

double Airfoil::getCl(double dAlfaDeg) {
    return m_AirfoilModel->getCl(dAlfaDeg);
}

double Airfoil::getCd(double dAlfaDeg) {
    return m_Cdfactor * m_AirfoilModel->getCd(dAlfaDeg) + m_CdDelta;
}

void Airfoil::setCdFactor(double CdFactor) {
    m_Cdfactor = CdFactor;
}

void Airfoil::setCdDelta(double CdDelta) {
    m_CdDelta = CdDelta;
}

double Airfoil::getReynoldsNumber() {
    return m_AirfoilModel->getReynoldsNumber();
}

double Airfoil::getMachNumber() {
    return m_AirfoilModel->getMachNumber();
}


