#include <string>
#include <vector>
#include "DoublePoint.h"

class AirfoilElement {
public:
    int PointCount;
    DoublePoint* Points;
    void setPoints(int count, DoublePoint* points) {
        PointCount = count;
        Points = points;
    }
    void getPoints(DoublePoint* output) {
        for (int i = 0; i < PointCount; i++) {
            output[i] = Points[i];
        }
    }
    void ReSpline(int newPointCount) {
        // Implement respline logic here
    }
    void setTEThickness(double thickness) {
        // Implement set TE thickness logic here
    }
};
