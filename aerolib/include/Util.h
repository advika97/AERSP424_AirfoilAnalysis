#ifndef _UTIL_H_
#define _UTIL_H_


#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

//! Typedef for convenience. This type stores the Xfoil results table.
using polar = std::unordered_map<std::string, std::vector<double>>;

class XFoilPolarParser {

public:
    XFoilPolarParser(const std::string& path);

    vector<double> getAlpha();
    vector<double> getCL();
    vector<double> getCD();
    // add other get methods
    pair<double, double> getMinMaxAlpha();
    pair<double, double> getMinMaxCL();
    pair<double, double> getMinMaxCD();
    long getReynolds();

private:
    polar table;
    long reynolds;
    vector<string> headers = { "alpha", "CL", "CD", "CDp", "CM", "Top_Xtr", "Bot_Xtr", "Top_Itr", "Bot_Itr" };

    void ParsePolar(const std::string& path);

};


#endif
