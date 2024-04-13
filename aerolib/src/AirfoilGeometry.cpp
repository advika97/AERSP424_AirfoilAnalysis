#include <string>
#include <vector>
#include "AirfoilGeometry.h"


AirfoilGeometry::AirfoilGeometry(const AirfoilGeometry& theSource) {
    Init();
    m_nElements = theSource.m_nElements;
    m_Area = theSource.m_Area;
    ptRef = DoublePoint(theSource.ptRef.dX, theSource.ptRef.dY);
    m_Name = theSource.m_Name;
    m_GroundEffect = theSource.m_GroundEffect;
    m_FroudeEffect = theSource.m_FroudeEffect;
    Element.resize(m_nElements);
    for (int j = 0; j < m_nElements; ++j) {
        Element[j] = AirfoilElement(theSource.Element[j]);
    }
    UpdateProps(ptRef);
}

void AirfoilGeometry::Init() {
    m_Name = "";
    m_nElements = 0;
    ptRef = DoublePoint(0.25, 0.0);
    m_GroundEffect = false;
    m_FroudeEffect = false;
}

int AirfoilGeometry::CountElements(int nPoints, double* X, double* Y) {
    int nElements = 1;
    for (int nPoint = 0; nPoint < nPoints; ++nPoint) {
        if (X[nPoint] > 998.0 || X[nPoint] < -998.0) {
            ++nElements;
        }
    }
    return nElements;
}


void AirfoilGeometry::setName(const std::string& Name) {
    m_Name = Name;
}

std::string AirfoilGeometry::getName() {
    return m_Name;
}

int AirfoilGeometry::getElementCount() {
    return m_GroundEffect ? m_nElements / 2 : m_nElements;
}

void AirfoilGeometry::setPoints(int nPoints, double* X, double* Y) {
    Init();
    if (nPoints > 0) {
        m_nElements = CountElements(nPoints, X, Y);
        Element.resize(m_nElements);
        int nOffset = 0;
        int nElement = 0;
        for (int nPoint = 0; nPoint < nPoints; ++nPoint) {
            if (X[nPoint] > 998.0 || X[nPoint] < -998.0 || nPoint == nPoints - 1) {
                Element[nElement].setPoints(nPoint - nOffset, X, Y, nOffset);
                nOffset = nPoint + 1;
                ++nElement;
            }
        }
        UpdateProps(ptRef);
    }
}

void AirfoilGeometry::setTEThickness(double newTEThickness) {
    for (int j = 0; j < m_nElements; ++j) {
        Element[j].setTEThickness(newTEThickness);
    }
}

void AirfoilGeometry::addElement(const AirfoilElement& theElement) {
    int nElements = getElementCount();
    Element.resize(nElements + 1);
    for (int nElement = 0; nElement < nElements; ++nElement) {
        Element[nElement] = this->Element[nElement];
    }
    Element[nElements] = theElement;
    ++m_nElements;
    if (m_GroundEffect) {
        setMirrorGeometry(true);
    }
    UpdateProps(ptRef);
}

void AirfoilGeometry::deleteElement(int nElementToDelete) {
    int nElements = getElementCount();
    if (nElementToDelete < nElements) {
        std::vector<AirfoilElement> NewElement(nElements - 1);

        int nElement;
        for (nElement = 0; nElement < nElementToDelete; ++nElement) {
            NewElement[nElement] = Element[nElement];
        }

        for (nElement = nElementToDelete; nElement < nElements - 1; ++nElement) {
            NewElement[nElement] = Element[nElement + 1];
        }

        Element = NewElement;
        m_nElements = nElements - 1;
        if (m_GroundEffect) {
            setMirrorGeometry(true);
        }

        UpdateProps(ptRef);
    }
}

bool AirfoilGeometry::fromFile(std::string filePath) {
    return fromFile(filePath, 61);
}

bool AirfoilGeometry::fromFile(std::string filePath, int pointCount) {
    try {
        int fmt = checkFile(filePath);
        std::ifstream f(filePath);
        if (fmt != FMT_PNG && fmt != FMT_GIF && fmt != FMT_JPG && fmt != FMT_BMP) {
            std::string s((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            if (fmt == FMT_GEO) {
                fromString(stripFileName(filePath), s);
            } else if (fmt == FMT_XML) {
                fromXML(s);
            }
        } else {
            fromImage(filePath, pointCount);
        }
    } catch (std::exception& e) {
        // Handle exceptions
    }

    return true;
}

int AirfoilGeometry::checkFile(std::string fileName) {
    int retVal = FMT_GEO;
    if (fileName.find(".txt") == std::string::npos && fileName.find(".geo") == std::string::npos) {
        if (fileName.find(".xml") != std::string::npos) {
            retVal = FMT_XML;
        } else if (fileName.find(".png") != std::string::npos) {
            retVal = FMT_PNG;
        } else if (fileName.find(".gif") != std::string::npos) {
            retVal = FMT_GIF;
        } else if (fileName.find(".jpg") != std::string::npos) {
            retVal = FMT_JPG;
        } else if (fileName.find(".bmp") != std::string::npos) {
            retVal = FMT_BMP;
        } else {
            try {
                std::ifstream f(fileName, std::ios::binary);
                char buffer[32];
                f.read(buffer, 32);
                if (buffer[6] == 74 && buffer[7] == 70 && buffer[8] == 73 && buffer[9] == 70) {
                    retVal = FMT_JPG;
                } else if (buffer[0] == -119 && buffer[1] == 80 && buffer[2] == 78 && buffer[3] == 71) {
                    retVal = FMT_PNG;
                } else if (buffer[0] == 71 && buffer[1] == 73 && buffer[2] == 70 && buffer[3] == 56 && buffer[4] == 57) {
                    retVal = FMT_GIF;
                } else {
                    std::string s(buffer, 32);
                    if (s.find("<?xml ") != std::string::npos) {
                        retVal = FMT_XML;
                    }
                }
            } catch (std::exception& e) {
                // Handle exceptions
            }
        }
    } else {
        retVal = FMT_GEO;
    }

    return retVal;
}

void AirfoilGeometry::fromString(std::string& defaultName, const std::string& buffer) {
    std::istringstream iss(buffer);
    std::string sLine;

    try {
        std::getline(iss, sLine);
        if (sLine.empty()) {
            return;
        }

        int nPoints = 0;
        std::vector<double> X(10);
        std::vector<double> Y(10);

        if (std::regex_match(sLine, std::regex(".*[A-DF-Za-df-z_%]+.*"))) {
            defaultName = sLine;
            std::getline(iss, sLine);
        }

        while (std::getline(iss, sLine)) {
            // Replace special characters and clean up the line
            size_t pos;
            while ((pos = sLine.find("\u001a")) != std::string::npos) {
                sLine.replace(pos, 1, " ");
            }
            while ((pos = sLine.find("  ")) != std::string::npos) {
                sLine.replace(pos, 2, " ");
            }
            while ((pos = sLine.find(",")) != std::string::npos) {
                sLine.replace(pos, 1, ".");
            }

            if (sLine.length() > 2) {
                std::istringstream lineStream(sLine);
                double x, y;
                if (lineStream >> x >> y) {
                    if (nPoints == X.size()) {
                        X.resize(2 * X.size());
                        Y.resize(2 * Y.size());
                    }
                    X[nPoints] = x;
                    Y[nPoints] = y;
                    ++nPoints;
                }
            }
        }

        // Set points and name after parsing
        setPoints(nPoints, X.data(), Y.data());
        m_Name = defaultName;
    } catch (const std::exception& ex) {
        // Handle exceptions as needed
    }
}

std::string AirfoilGeometry::toXML() {
    int nElements = getElementCount();
    std::string sLineTerminator = "\n";
    std::string xmlResult;

    try {
        // Get system line separator
        sLineTerminator = "\n"; // Use '\n' for simplicity

        std::ostringstream sb;
        sb << "<?xml version=\"1.0\"?>" << sLineTerminator;
        sb << "<airfoil xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"airfoil-schema.xsd\">" << sLineTerminator;
        sb << "  <name>" << m_Name << "</name>" << sLineTerminator;
        sb << "  <elements>" << sLineTerminator;

        for (int j = 0; j < nElements; ++j) {
            sb << Element[j].toXML("    "); // Assuming Element is an array of AirfoilElement
        }

        sb << "  </elements>" << sLineTerminator;
        sb << "</airfoil>" << sLineTerminator;

        xmlResult = sb.str();
    } catch (const std::exception& ex) {
        // Handle exceptions as needed
    }

    return xmlResult;
}

bool AirfoilGeometry::fromImage(const std::string& fileName, int pointCount) {
    bool retVal = false;

    try {
        Init();
        m_nElements = 1;
        Element = new AirfoilElement[1]; // Assuming Element is an array of AirfoilElement
        Element[0].fromImage(fileName, 10, 4, pointCount);
        m_Name = "sampled 10-4";
        retVal = true;
    } catch (const std::exception& ex) {
        // Handle exceptions as needed
    }

    return retVal;
}



std::string AirfoilGeometry::stripFileName(std::string filePath) {
    // Implement stripFileName method
    return "";
}

bool AirfoilGeometry::fromXML(const std::string& buffer) {
    XMLBlock b(buffer);
    XMLBlock bName = b.getFirstElement("name");
    std::string sName = bName.getContent();
    std::ostringstream sb;
    XMLBlock bElement = b.getFirstElement("element");

    while (!bElement.isEmpty()) {
        XMLBlock bCoordinates = bElement.getFirstElement("coordinates");

        for (XMLBlock bPoint = bCoordinates.getFirstElement("point"); !bPoint.isEmpty(); bPoint = bCoordinates.getNextElement("point")) {
            XMLBlock x = bPoint.getFirstElement("x");
            XMLBlock y = bPoint.getFirstElement("y");
            if (!x.isEmpty() && !y.isEmpty()) {
                sb << x.getContent() << " " << y.getContent() << "\n";
            }
        }

        bElement = b.getNextElement("element");
        if (!bElement.isEmpty()) {
            sb << "999.9 999.9\n";
        }
    }

    fromString(sName, sb.str());
    return true;
}

std::string AirfoilGeometry::toGeo() const {
    std::ostringstream sb;
    sb << m_Name << "\n";

    for (int j = 0; j < m_nElements; ++j) {
        sb << Element[j].toGeo() << "999.9 999.9\n";
    }

    return sb.str();
}

std::string AirfoilGeometry::toJava() const {
    std::ostringstream sb;
    sb << "/*\n";
    sb << " * " << m_Name << "\n";
    sb << " */\n";

    for (int j = 0; j < m_nElements; ++j) {
        sb << "// element " << (j + 1) << "\n";
        sb << Element[j].toJava(j + 1);
    }

    return sb.str();
}

std::string AirfoilGeometry::toJavaDP() const {
    std::ostringstream sb;
    sb << "/*\n";
    sb << " * " << m_Name << "\n";
    sb << " */\n";

    for (int j = 0; j < m_nElements; ++j) {
        sb << "// element " << (j + 1) << "\n";
        sb << Element[j].toJava(j + 1);
    }

    return sb.str();
}

bool AirfoilGeometry::IsPointInside(const DoublePoint& pt) const {
    for (int j = 0; j < m_nElements; ++j) {
        double YU = Element[j].getUpperY(pt.dX);
        if (YU > -999.0) {
            double YL = Element[j].getLowerY(pt.dX);
            if (YL > -999.0 && pt.dY > YL && pt.dY < YU) {
                return true;
            }
        }
    }
    return false;
}

void AirfoilGeometry::ReSpline(int NewPointCount) {
    int nElements = getElementCount();
    std::vector<AirfoilElement> NewElement(nElements);

    for (int nElement = 0; nElement < nElements; ++nElement) {
        NewElement[nElement] = Element[nElement];
        NewElement[nElement].ReSpline(NewPointCount);
    }

    Element = new AirfoilElement[nElements];
    std::copy(NewElement.begin(), NewElement.end(), Element);

    if (m_GroundEffect) {
        setMirrorGeometry(true);
    }

    UpdateProps({0.0, 0.0}); // Assuming ptRef is DoublePoint({0.0, 0.0})
}

void AirfoilGeometry::setFroudeEffect(bool Active) {
    if (Active) {
        setMirrorGeometry(false);
    }
    m_FroudeEffect = Active;
}

void AirfoilGeometry::setGroundEffect(bool Active) {
    if (Active) {
        m_FroudeEffect = false;
    }
    setMirrorGeometry(Active);
    m_GroundEffect = Active;
}

bool AirfoilGeometry::IsGroundEffect() const {
    return m_GroundEffect;
}

bool AirfoilGeometry::IsFroudeEffect() const {
    return m_FroudeEffect;
}

DoublePoint AirfoilGeometry::getBounds(int boundsType) const {
    DoublePoint ptBounds = Element[0].getBounds(boundsType);

    for (int i = 1; i < getElementCount(); ++i) {
        DoublePoint ptElementBounds = Element[i].getBounds(boundsType);
        if (boundsType == 0) {
            ptBounds.dX = std::min(ptBounds.dX, ptElementBounds.dX);
            ptBounds.dY = std::min(ptBounds.dY, ptElementBounds.dY);
        } else {
            ptBounds.dX = std::max(ptBounds.dX, ptElementBounds.dX);
            ptBounds.dY = std::max(ptBounds.dY, ptElementBounds.dY);
        }
    }

    return ptBounds;
}


void AirfoilGeometry::setMirrorGeometry(bool Active) {
    if (Active && !m_GroundEffect) {
        int nElements = m_nElements;
        DoublePoint ptGround = {0.0, 0.0};

        std::vector<AirfoilElement> newElements;
        for (int nElement = 0; nElement < nElements; ++nElement) {
            AirfoilElement theElement = Element[nElement];
            theElement.FlipY(ptGround);
            newElements.push_back(theElement);
        }

        m_nElements += newElements.size();
        Element = new AirfoilElement[m_nElements];
        std::copy(newElements.begin(), newElements.end(), Element);
    } else if (!Active && m_GroundEffect) {
        // Revert to original elements when disabling ground effect
        m_nElements = getElementCount();
        delete[] Element; // Clean up existing elements
    }
}

void AirfoilGeometry::UpdateProps(const DoublePoint& ptRef) {
    m_Area = 0.0;
    for (int j = 0; j < m_nElements; ++j) {
        Element[j].UpdateProps(ptRef.dX, ptRef.dY);
        m_Area += Element[j].Area; // Assuming Area is a member variable of AirfoilElement
    }
}


