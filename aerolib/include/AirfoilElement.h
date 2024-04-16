#ifndef _AIRFOILELEMENT_H_
#define _AIRFOILELEMENT_H_

#define _USE_MATH_DEFINES
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "DoublePoint.h"
#include "Circle.h"
#include "RationalSpline.h"

using namespace std;

class AirfoilElement {

public:
    int PointCount;
    vector<DoublePoint> Points;
    vector<double> ArcLength;
    double Area;
    DoublePoint Inertia;
    DoublePoint Center;
    DoublePoint ptCamber;
    DoublePoint ptThickness;
    double Cl;
    double Cd;
    double Cm;
    static constexpr int BOUNDS_MIN = 0;
    static constexpr int BOUNDS_MAX = 1;

private:
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    vector<DoublePoint> m_Thickness;
    vector<DoublePoint> m_CamberLine;
    vector<double> m_Curvature;
    int m_LEIndex;
    int m_Recursive = 0;
    static constexpr double MIN_DISTANCE = 1.0E-16;

    void InitElement();
    void BSpline(int nPolynomDegree, vector<DoublePoint>& ptControl);
    void ComputeIntervals(vector<int>& u, int n, int t);
    DoublePoint compute_point(vector<int>& u, int n, int t, double v, vector<DoublePoint>& control);
    void UpdateCamberAndThickness();
    double blend(int k, int t, vector<int>& u, double v);
    static void RotatePoints(vector<DoublePoint>& Points, DoublePoint Pivot, double DeltaAngle);

public:

    AirfoilElement();
    AirfoilElement(int nPoints);
    AirfoilElement(const AirfoilElement& theSource);
    AirfoilElement(vector<DoublePoint>& ptControl, int nDegree, int nPoints);

    void UpdateProps(DoublePoint& Center);
    void UpdateProps(double dXs, double dYs);
    void ReversePointSequence();
    double getLowerY(double x);
    double getUpperY(double x);
    void setPoints(int nPoints, vector<double>& X, vector<double>& Y);
    void setPoints(int nPoints, vector<double>& X, vector<double>& Y, int nOffset);
    void setPoints(int nPoints, vector<DoublePoint>& pt);
    void setPoints(int nPoints, vector<DoublePoint>& pt, int nOffset);
    bool getPoints(vector<double>& X, vector<double>& Y);
    bool getPoints(vector<double>& X, vector<double>& Y, int nOffset);
    bool getPoints(vector<DoublePoint>& pts);
    bool getPoints(vector<DoublePoint>& pts, int nOffset);
    double getTEThickness();
    void setTEThickness(double newTEThickness);
    void ReSpline(int NewPointCount);
    void ReSpline(int NewPointCount, double weight);
    DoublePoint getBounds(int boundsType);
    void FlipY(DoublePoint Pivot);
};

#endif
