// Spline.h

#ifndef SPLINE_H
#define SPLINE_H

class Spline {
public:
    virtual double EvalPoint(double var1) const = 0;
    virtual ~Spline() {} // Virtual destructor for polymorphic behavior
};

#endif
