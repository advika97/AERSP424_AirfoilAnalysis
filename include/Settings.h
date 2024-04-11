#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <wx/wx.h>
#include <vector>
#include <algorithm>
// #include <execution>

using namespace std;

// Structure pour représenter un point en 2D
struct Point {
    double x;
    double y;
};

// Fonction de comparaison de points selon leur abscisse
// bool comparePoints(const Point &a, const Point &b);

vector<wxPoint> getSquarePoints(wxPoint pos, double length);
vector<Point> getSquarePoints(Point pos, double length);

vector<wxPoint> getNacaPoints(wxPoint pos, double length);
vector<Point> getNacaPoints(Point pos, double length); 
vector<Point> generateNacaProfile(Point pos, double chord, double thickness, int nb_points, double incidence=M_PI/12);
vector<wxPoint> generateNacaProfile(wxPoint pos, double chord, double thickness, int nb_points, double incidence=M_PI/12);


wxPoint *fromVectorToPtr(vector<wxPoint> pt);

// bool isPointInPolygon(vector<wxPoint> P, wxPoint M);

// bool isInside(vector<Point> polygon, Point P);
bool isInsidePolygon(vector<Point> polygon, Point P);
vector<Point> rotatePolygon(vector<Point> polygon, Point center, double theta);
vector<wxPoint> rotatePolygon(vector<wxPoint> polygon, wxPoint center, double theta);


#endif // SETTINGS_H_INCLUDED
