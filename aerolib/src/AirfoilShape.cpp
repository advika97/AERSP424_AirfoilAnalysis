#include "AirfoilShape.h"
#include <cmath> // For std::hypot
#include <sstream>

// Static member initialization
std::vector<AirfoilParameter> AirfoilShape::m_parameters;

void AirfoilShape::addParameter(const AirfoilParameter& parameter)
{
}

AirfoilShape::~AirfoilShape() {
    // Clean up dynamically allocated arrays
    delete[] m_X;
    delete[] m_Y;
}

std::string AirfoilShape::getName() const {
    return m_sName;
}

std::string AirfoilShape::getTypeName() const {
    return m_TypeName;
}

std::string AirfoilShape::getComment() const {
    return m_Comment;
}

void AirfoilShape::allocatePoints(int nPoints) {
    m_X = new double[nPoints];
    m_Y = new double[nPoints];
}

int AirfoilShape::getPointCount() const {
    return m_X ? (sizeof(m_X) / sizeof(*m_X)) : 0;
}

double* AirfoilShape::getX() const {
    return m_X;
}

double* AirfoilShape::getY() const {
    return m_Y;
}

double* AirfoilShape::getS() const {
    double* s = new double[getPointCount()];
    s[0] = 0.0;

    for (int i = 1; i < getPointCount(); ++i) {
        s[i] = s[i - 1] + std::hypot(m_X[i] - m_X[i - 1], m_Y[i] - m_Y[i - 1]);
    }

    return s;
}

void AirfoilShape::closeShape() {
    int PointCount = getPointCount();
    if (PointCount > 1) {
        m_X[PointCount - 1] = m_X[0] = 0.5 * (m_X[0] + m_X[PointCount - 1]);
        double dY = (m_Y[0] - m_Y[PointCount - 1]) / 2.0;

        int i = 0;
        for (; i < PointCount - 1 && !(m_X[i + 1] > m_X[i]); ++i) {
            m_Y[i] -= dY * (m_X[i] / m_X[0]);
        }

        while (i < PointCount) {
            m_Y[i] += dY * (m_X[i] / m_X[0]);
            ++i;
        }
    }
}

std::string AirfoilShape::toString() const {
    std::ostringstream sb;
    sb << m_sName << "\n";

    for (int i = 0; i < getPointCount(); ++i) {
        sb << m_X[i] << "\t" << m_Y[i] << "\n";
    }

    return sb.str();
}
