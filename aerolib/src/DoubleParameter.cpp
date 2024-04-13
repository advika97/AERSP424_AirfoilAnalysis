#include "DoubleParameter.h"
#include <sstream>

DoubleParameter::DoubleParameter(const std::string& name, const std::string& unit, double value, double min, double max, double step)
    : AirfoilParameter(name), m_unit(unit), m_value(value), m_min(min), m_max(max), m_step(step) {}

