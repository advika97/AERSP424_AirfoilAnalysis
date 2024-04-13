#ifndef AIRFOILPARAMETER_H
#define AIRFOILPARAMETER_H

#include <string>

class AirfoilParameter {
protected:
    std::string m_name;

public:
    AirfoilParameter(const std::string& name = "") : m_name(name) {}
    ~AirfoilParameter() = default;

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
};

#endif // AIRFOILPARAMETER_H
