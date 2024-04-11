
#ifndef AIRFOIL_APP_H
#define AIRFOIL_APP_H

#include <wx/wx.h>
#include "AirfoilFrame.h"

class AirfoilApp : public wxApp {
public:
    bool OnInit() override;

private:
    AirfoilFrame* frame;
};

#endif // AIRFOIL_APP_H
