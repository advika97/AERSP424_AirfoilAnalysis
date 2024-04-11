#include "Settings.h"


vector<wxPoint> getSquarePoints(wxPoint pos, double length)
{
    return {wxPoint(pos.x - length, pos.y + length),
            wxPoint(pos.x + length, pos.y + length),
            wxPoint(pos.x + length, pos.y - length),
            wxPoint(pos.x - length, pos.y - length)};
}

vector<Point> getSquarePoints(Point pos, double length)
{
    return {{pos.x - length, pos.y + length},
            {pos.x + length, pos.y + length},
            {pos.x + length, pos.y - length},
            {pos.x - length, pos.y - length}};
}


vector<wxPoint> getNacaPoints(wxPoint pos, double length)
{
    return generateNacaProfile(pos, 4.0 * length, 0.12, 10, M_PI / 12);
}

vector<Point> getNacaPoints(Point pos, double length)
{
    return generateNacaProfile(pos, 4.0 * length, 0.12, 10, -M_PI / 12);
}

wxPoint *fromVectorToPtr(vector<wxPoint> pt)
{
    wxPoint *ptr = new wxPoint[pt.size()];
    for (size_t i = 0; i < pt.size(); i++)
        ptr[i] = pt[i];
    return ptr;
}

bool isInsidePolygon(vector<Point> polygon, Point P)
{
    int count = 0;
    for (int i = 0; i < polygon.size(); i++)
    {
        Point A = polygon[i];
        Point B = polygon[(i + 1) % polygon.size()];

        if ((P.y > min(A.y, B.y)) && (P.y <= max(A.y, B.y)))
        {
            double x = A.x + (P.y - A.y) * (B.x - A.x) / (B.y - A.y);

            if (x > P.x)
            {
                count++;
            }
        }
    }
    return count & 1; //count % 2 == 1;
}

vector<Point> generateNacaProfile(Point pos, double chord, double thickness, int nb_points, double incidence)
{
    vector<Point> points;
    int contraction = 3;
    double c = chord;
    double t = thickness;
    int n = nb_points;

    for (int i = 0; i <= n; i++)
    {
        double x = c * pow(double(i) / n, contraction);
        double y = (c * t / 0.2) * (0.2969 * sqrt(x / c) - 0.1260 * (x / c) - 0.3516 * pow(x / c, 2) + 0.2843 * pow(x / c, 3) - 0.1015 * pow(x / c, 4));
        points.push_back({x + pos.x - c / 2, y + pos.y});
    }
    for (int i = n; i >= 0; i--)
    {
        double x = c * pow(double(i) / n, contraction);
        double y = (c * t / 0.2) * (0.2969 * sqrt(x / c) - 0.1260 * (x / c) - 0.3516 * pow(x / c, 2) + 0.2843 * pow(x / c, 3) - 0.1015 * pow(x / c, 4));
        points.push_back({x + pos.x - c / 2, -y + pos.y});
    }

    return rotatePolygon(points, pos, incidence);
}

vector<wxPoint> generateNacaProfile(wxPoint pos, double chord, double thickness, int nb_points, double incidence)
{
    vector<wxPoint> points;
    int contraction = 3;
    double c = chord;
    double t = thickness;
    int n = nb_points;

    for (int i = 0; i <= n; i++)
    {
        double x = c * pow(double(i) / n, contraction);
        double y = (c * t / 0.2) * (0.2969 * sqrt(x / c) - 0.1260 * (x / c) - 0.3516 * pow(x / c, 2) + 0.2843 * pow(x / c, 3) - 0.1015 * pow(x / c, 4));
        points.push_back(wxPoint(x + pos.x - c / 2, y + pos.y));
    }
    for (int i = n; i >= 0; i--)
    {
        double x = c * pow(double(i) / n, contraction);
        double y = (c * t / 0.2) * (0.2969 * sqrt(x / c) - 0.1260 * (x / c) - 0.3516 * pow(x / c, 2) + 0.2843 * pow(x / c, 3) - 0.1015 * pow(x / c, 4));
        points.push_back(wxPoint(x + pos.x - c / 2, -y + pos.y));
    }

    return rotatePolygon(points, pos, incidence);
}


vector<Point> rotatePolygon(vector<Point> polygon, Point center, double theta)
{
    vector<Point> rotatedPolygon;
    for (int i = 0; i < polygon.size(); i++)
    {
        double x = center.x + (polygon[i].x - center.x) * cos(theta) - (polygon[i].y - center.y) * sin(theta);
        double y = center.y + (polygon[i].x - center.x) * sin(theta) + (polygon[i].y - center.y) * cos(theta);
        rotatedPolygon.push_back({x, y});
    }

    return rotatedPolygon;
}

vector<wxPoint> rotatePolygon(vector<wxPoint> polygon, wxPoint center, double theta)
{
    vector<wxPoint> rotatedPolygon;
    for (int i = 0; i < polygon.size(); i++)
    {
        double x = center.x + (polygon[i].x - center.x) * cos(theta) - (polygon[i].y - center.y) * sin(theta);
        double y = center.y + (polygon[i].x - center.x) * sin(theta) + (polygon[i].y - center.y) * cos(theta);
        rotatedPolygon.push_back(wxPoint(x, y));
    }

    return rotatedPolygon;
}

