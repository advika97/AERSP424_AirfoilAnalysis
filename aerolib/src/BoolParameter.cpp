#include "BoolParameter.h"

BoolParameter::BoolParameter(const std::string& name, bool value)
    : AirfoilParameter(name), m_value(value) {}

