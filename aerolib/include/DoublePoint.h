#include <string>
#include <cmath>
#include <locale>
#include <sstream>
#include <iomanip>

class DoublePoint {
public:
    double dX;
    double dY;

    DoublePoint();
    DoublePoint(double X, double Y); 
    DoublePoint(const DoublePoint& ptBase);
    void subtract(double X, double Y) ;
    void subtract(const DoublePoint& ptSub);
    void add(double X, double Y);
    void add(const DoublePoint& ptAdd);

    void scale(const DoublePoint& ptMultiply) ;

    void scale(double dFactor) ;

    void max(double X, double Y);

    void max(const DoublePoint& ptMax);

    void min(double X, double Y);

    void min(const DoublePoint& ptMin);

    double getLength();
    void setLength(double Length);

    void Normalize() ;

    static void arraycopy(DoublePoint* ptSrc, int nSrcOffset, DoublePoint* ptDst, int nDstOffset, int nLength);

    static DoublePoint* allocateArray(int nElements) ;
};

