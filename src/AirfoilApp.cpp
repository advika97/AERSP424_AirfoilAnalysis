#include "AirfoilApp.h"

wxIMPLEMENT_APP(AirfoilApp);

bool AirfoilApp::OnInit() {
    frame = new AirfoilFrame(0L);
    frame->Show(true);
    return true;
}

