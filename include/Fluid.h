#ifndef FLUID_H
#define FLUID_H

#include <memory>
#include <vector>
#include <math.h>
#include <algorithm>

#include "Settings.h"

#define U_FIELD  0
#define V_FIELD  1
#define S_FIELD  2

/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

/* Some example here */
#pragma message(VAR_NAME_VALUE(_OPENMP))

using namespace std;

#if _OPENMP >= 201811
#pragma omp declare reduction(vec_double_plus : std::vector<double> : \
                              transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), plus<double>())) \
                    initializer(omp_priv = decltype(omp_orig)(omp_orig.size()))
#endif

class Fluid
{
public:
    Fluid(double _density, int _numX, int _numY, double _h, double _overRelaxation=1.9, int _numThreads=4);

// ----------------- start of simulator ------------------------------
    void integrate(double dt, double gravity);
    void solveIncompressibility(int numIters, double dt);
    void extrapolate();
    double sampleField(double x, double y, int field);
    double avgU(int i, int j);
    double avgV(int i, int j);
    void computeVelosityMagnitude();
    void advectVelocity(double dt);
    void advectTracer(double dt);
    void simulate(double dt, double gravity, int numIters);
// ----------------- end of simulator ------------------------------

    void updateFluidParameters();

    int cnt;

    double density=1000.0;
    int numX;
    int numY;
    int numCells;
    int num;
    double h;
    double overRelaxation;
    int numThreads=4;
    vector<double> u;
    vector<double> v;
    vector<double> Vel;
    vector<double> newU;
    vector<double> newV;
    vector<double> p;
    vector<double> s;
    vector<double> m;
    vector<double> newM;
};

#endif // FLUID_H
