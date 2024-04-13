#ifndef BOOLPARAMETER_H
#define BOOLPARAMETER_H

#include "AirfoilParameter.h" // Include base class header

class BoolParameter : public AirfoilParameter {
private:
    bool m_value;

public:
    BoolParameter(const std::string& name, bool value);
    virtual ~BoolParameter() = default;

    bool getValue() const { return m_value; }
    void setValue(bool value) { m_value = value; }

};

#endif // BOOLPARAMETER_H
