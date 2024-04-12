
#include "Util.h"

XFoilPolarParser::XFoilPolarParser(const std::string& path) {
    ParsePolar(path);
}


vector<double> XFoilPolarParser::getAlpha() {
    return table[headers[0]];
}

vector<double> XFoilPolarParser::getCL() {
    return table[headers[1]];
}

vector<double> XFoilPolarParser::getCD() {
    return table[headers[2]];
}

pair<double, double> XFoilPolarParser::getMinMaxAlpha() {
    vector<double> alpha = getAlpha();
    return pair<double, double>(*min_element(alpha.begin(), alpha.end()), *max_element(alpha.begin(), alpha.end()));
}

pair<double, double> XFoilPolarParser::getMinMaxCL() {
    vector<double> cl = getCL();
    return pair<double, double> (*min_element(cl.begin(), cl.end()), *max_element(cl.begin(), cl.end()));
}

pair<double, double> XFoilPolarParser::getMinMaxCD() {
    vector<double> cd = getCD();
    return pair<double, double> (*min_element(cd.begin(), cd.end()), *max_element(cd.begin(), cd.end()));
}

long XFoilPolarParser::getReynolds() {
    return reynolds;
}

void XFoilPolarParser::ParsePolar(const std::string& path) {

  for (const auto &key: headers) {
    table[key] = std::vector<double>();
  }
  // number of lines in polar before header
  constexpr size_t kHeader = 12;
  std::string line;
  std::ifstream file;
  file.open(path);
  size_t line_nr = 0;
  while (std::getline(file, line)) {
      if (line_nr == 8) {   // 9th line has mach, reynolds and ncrit
            // split the line by white spaces
            std::istringstream iss(line);
            std::vector<std::string> vec = std::vector<std::string>(std::istream_iterator<std::string>(iss),
                                                            std::istream_iterator<std::string>());
            reynolds = (long) (stod(vec[5]) * pow(10.0, stod(vec[7])));
      }
    // skip the header
    if (line_nr < kHeader) {
      line_nr++;
      continue;
    }
    // split the line by white spaces
    std::istringstream iss(line);
    std::vector<std::string> vec = std::vector<std::string>(std::istream_iterator<std::string>(iss),
                                                            std::istream_iterator<std::string>());
    for (size_t i=0; i<vec.size(); i++) {
      table[headers[i]].push_back(std::stod(vec[i]));
    }
  }
}
