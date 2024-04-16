#include <string>
#include <vector>
#include "AirfoilGeometry.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

AirfoilGeometry::AirfoilGeometry(const AirfoilGeometry& theSource) {
    Init();
    m_nElements = theSource.m_nElements;
    m_Area = theSource.m_Area;
    ptRef = DoublePoint(theSource.ptRef);
    m_Name = theSource.m_Name;
    m_GroundEffect = theSource.m_GroundEffect;
    m_FroudeEffect = theSource.m_FroudeEffect;
    Element = theSource.Element;
    UpdateProps(ptRef);
}

void AirfoilGeometry::Init() {
    m_Name = "";
    m_nElements = 0;
    ptRef = DoublePoint(0.25, 0.0);
    m_GroundEffect = false;
    m_FroudeEffect = false;
}

int AirfoilGeometry::CountElements(int nPoints, vector<double>& X, vector<double>& Y) {
    int nElements = 1;
    for (int nPoint = 0; nPoint < nPoints; ++nPoint) {
        if (X[nPoint] > 998.0 || X[nPoint] < -998.0) {
            ++nElements;
        }
    }
    return nElements;
}


void AirfoilGeometry::setName(const std::string& Name) {
    m_Name = Name;
}

std::string AirfoilGeometry::getName() {
    return m_Name;
}

int AirfoilGeometry::getElementCount() {
    return m_GroundEffect ? m_nElements / 2 : m_nElements;
}

void AirfoilGeometry::setPoints(int nPoints, vector<double>& X, vector<double>& Y) {
    Init();
    if (nPoints > 0) {
        m_nElements = CountElements(nPoints, X, Y);
        Element.resize(m_nElements);
        int nOffset = 0;
        int nElement = 0;
        for (int nPoint = 0; nPoint < nPoints; ++nPoint) {
            if (X[nPoint] > 998.0 || X[nPoint] < -998.0 || nPoint == nPoints - 1) {
                Element[nElement].setPoints(nPoint - nOffset, X, Y, nOffset);
                nOffset = nPoint + 1;
                ++nElement;
            }
        }
        UpdateProps(ptRef);
    }
}

void AirfoilGeometry::setTEThickness(double newTEThickness) {
    for (int j = 0; j < m_nElements; ++j) {
        Element[j].setTEThickness(newTEThickness);
    }
}

void AirfoilGeometry::addElement(const AirfoilElement& theElement) {
    Element.push_back(theElement);
    ++m_nElements;
    if (m_GroundEffect) {
        setMirrorGeometry(true);
    }
    UpdateProps(ptRef);
}

void AirfoilGeometry::deleteElement(int nElementToDelete) {
    Element.erase(Element.begin() + nElementToDelete);
    m_nElements--;
    if (m_GroundEffect) {
        setMirrorGeometry(true);
    }
    UpdateProps(ptRef);

}


std::string AirfoilGeometry::stripFileName(std::string filePath) {
    // Implement stripFileName method
    std::filesystem::path p(filePath);
    return p.stem().string();
}


bool AirfoilGeometry::IsPointInside(const DoublePoint& pt) const {
    for (int j = 0; j < m_nElements; ++j) {
        double YU = ((AirfoilElement)Element[j]).getUpperY(pt.dX);
        if (YU > -999.0) {
            double YL = ((AirfoilElement)Element[j]).getLowerY(pt.dX);
            if (YL > -999.0 && pt.dY > YL && pt.dY < YU) {
                return true;
            }
        }
    }
    return false;
}

void AirfoilGeometry::ReSpline(int NewPointCount) {
    int nElements = getElementCount();
    std::vector<AirfoilElement> NewElement = Element;

    for (int nElement = 0; nElement < nElements; ++nElement) {
        NewElement[nElement].ReSpline(NewPointCount);
    }

    Element = NewElement;

    if (m_GroundEffect) {
        setMirrorGeometry(true);
    }

    UpdateProps(DoublePoint(0.0, 0.0)); // Assuming ptRef is DoublePoint({0.0, 0.0})
}

void AirfoilGeometry::setFroudeEffect(bool Active) {
    if (Active) {
        setMirrorGeometry(false);
    }
    m_FroudeEffect = Active;
}

void AirfoilGeometry::setGroundEffect(bool Active) {
    if (Active) {
        m_FroudeEffect = false;
    }
    setMirrorGeometry(Active);
    m_GroundEffect = Active;
}

bool AirfoilGeometry::IsGroundEffect() const {
    return m_GroundEffect;
}

bool AirfoilGeometry::IsFroudeEffect() const {
    return m_FroudeEffect;
}

DoublePoint AirfoilGeometry::getBounds(int boundsType) {
    DoublePoint ptBounds = ((AirfoilElement)Element[0]).getBounds(boundsType);

    for (int i = 1; i < getElementCount(); ++i) {
        DoublePoint ptElementBounds = Element[i].getBounds(boundsType);
        if (boundsType == 0) {
            ptBounds.dX = std::min(ptBounds.dX, ptElementBounds.dX);
            ptBounds.dY = std::min(ptBounds.dY, ptElementBounds.dY);
        } else {
            ptBounds.dX = std::max(ptBounds.dX, ptElementBounds.dX);
            ptBounds.dY = std::max(ptBounds.dY, ptElementBounds.dY);
        }
    }

    return ptBounds;
}


void AirfoilGeometry::setMirrorGeometry(bool Active) {
    if (Active && !m_GroundEffect) {
        int nElements = m_nElements;
        DoublePoint ptGround = {0.0, 0.0};

        std::vector<AirfoilElement> newElements;
        for (int nElement = 0; nElement < nElements; ++nElement) {
            AirfoilElement theElement = Element[nElement];
            theElement.FlipY(ptGround);
            newElements.push_back(theElement);
        }

        m_nElements += (int)newElements.size();
        Element = newElements;
    } else if (!Active && m_GroundEffect) {
        // Revert to original elements when disabling ground effect
        m_nElements = getElementCount();
        Element = vector<AirfoilElement>(); // Clean up existing elements
    }
}

void AirfoilGeometry::UpdateProps(std::string ptRef)
{
}

void AirfoilGeometry::UpdateProps(const DoublePoint& ptRef) {
    m_Area = 0.0;
    for (int j = 0; j < m_nElements; ++j) {
        Element[j].UpdateProps(ptRef.dX, ptRef.dY);
        m_Area += Element[j].Area; // Assuming Area is a member variable of AirfoilElement
    }
}


