class TabulatedAirfoil : public AirfoilModel {
public:
    TabulatedAirfoil(std::string BaseDir) {}
    bool Init(int nAirfoilIndex) override { return true; }
    double getCl(double dAlfaDeg) override { return 0.0; }
    double getCd(double dAlfaDeg) override { return 0.0; }
    double getReynoldsNumber() override { return 0.0; }
    double getMachNumber() override { return 0.0; }
    void setTEThickness(double TEThicknessPercent) override {}
};