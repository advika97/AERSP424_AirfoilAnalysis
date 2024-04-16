#ifndef RATIONAL_SPLINE_H
#define RATIONAL_SPLINE_H

#include "Spline.h" // Include the base interface
#include <vector>

class RationalSpline : public Spline {
public:
    RationalSpline(const std::vector<double>& x, const std::vector<double>& y);
    RationalSpline(const std::vector<double>& x, const std::vector<double>& y, double GlobalWeight);
    RationalSpline(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& w);

    virtual double EvalPoint(double x) const override;
    const std::vector<double>& getA() const;
    const std::vector<double>& getB() const;
    const std::vector<double>& getC() const;
    const std::vector<double>& getD() const;

private:
    int m_N;
    std::vector<double> m_Y;
    std::vector<double> m_X;
    std::vector<double> m_P;
    std::vector<double> m_Q;
    std::vector<double> m_Y1;
    std::vector<double> m_A;
    std::vector<double> m_B;
    std::vector<double> m_C;
    std::vector<double> m_D;

    void create(const std::vector<double>& x, const std::vector<double>& y);
    double phi(double U, double V, double P) const;
};

#endif
