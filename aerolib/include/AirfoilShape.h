#ifndef AIRFOILSHAPE_H
#define AIRFOILSHAPE_H

#include <vector>
#include <string>
#include "AirfoilParameter.h"

class AirfoilShape {
protected:
    double* m_X;
    double* m_Y;
    std::string m_sName;
    std::string m_Comment;
    std::string m_TypeName;
    static std::vector<AirfoilParameter> m_parameters;

    static void addParameter(const AirfoilParameter& parameter);

public:
    virtual ~AirfoilShape(); // Virtual destructor for polymorphism

    std::string getName() const;
    std::string getTypeName() const;
    std::string getComment() const;

    void allocatePoints(int nPoints);
    int getPointCount() const;
    double* getX() const;
    double* getY() const;

    double* getS() const;
    void closeShape();

    std::string toString() const;

    virtual std::vector<AirfoilParameter> getParameters() const = 0; // Pure virtual function
};

#endif // AIRFOILSHAPE_H
