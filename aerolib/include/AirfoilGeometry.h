#ifndef _AIRFOILGEOMETRY_H_
#define _AIRFOILGEOMETRY_H_

#include <string>
#include <vector>
#include "AirfoilElement.h"
#include "DoublePoint.h"

using namespace std;

class AirfoilGeometry {
private:
    std::string m_Name;
    int m_nElements;
    double m_Area;
    bool m_GroundEffect;
    bool m_FroudeEffect;
    static const int FMT_GEO = 0;
    static const int FMT_XML = 1;
    static const int FMT_PNG = 2;
    static const int FMT_GIF = 3;
    static const int FMT_BMP = 4;
    static const int FMT_JPG = 5;

public:
    std::vector<AirfoilElement> Element;
    DoublePoint ptRef;

    AirfoilGeometry() {
        Init();
    }

    AirfoilGeometry(const AirfoilGeometry& theSource) ;

private:
    void Init() ;
    int CountElements(int nPoints, vector<double>& X, vector<double>& Y) ;

    void UpdateProps(const DoublePoint& ref);
    //void setMirrorGeometry(bool mirror) ;

public:
    void setName(const std::string& Name);

    std::string getName() ;

    int getElementCount() ;

    void setPoints(int nPoints, vector<double>& X, vector<double>& Y);
    void setTEThickness(double newTEThickness) ;

    void addElement(const AirfoilElement& theElement);
    
    void deleteElement(int nElementToDelete) ;

    void setMirrorGeometry(bool b) ;

    void UpdateProps(std::string ptRef) ;

    std::string stripFileName(std::string filePath);

    bool IsPointInside(const DoublePoint& pt) const;

    void ReSpline(int NewPointCount) ;
    void setFroudeEffect(bool Active) ;
    void setGroundEffect(bool Active) ;

    bool IsGroundEffect() const ;
    bool IsFroudeEffect() const ;

    DoublePoint getBounds(int boundsType) ;

    
};

#endif
