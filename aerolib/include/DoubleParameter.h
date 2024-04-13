#ifndef DOUBLEPARAMETER_H
#define DOUBLEPARAMETER_H

#include "AirfoilParameter.h" // Include base class header
#include <string>

class DoubleParameter : public AirfoilParameter {
private:
    std::string m_unit;
    double m_value;
    double m_min;
    double m_max;
    double m_step;

public:
    DoubleParameter(const std::string& name, const std::string& unit, double value, double min, double max, double step);
    virtual ~DoubleParameter() = default;

    const std::string& getUnit() const { return m_unit; }
    double getValue() const { return m_value; }
    double getMin() const { return m_min; }
    double getMax() const { return m_max; }
    double getStep() const { return m_step; }

    void setValue(double value) { m_value = value; }

};

#endif // DOUBLEPARAMETER_H
