#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "AirfoilFrame.h"
#include "Util.h"

// Function to calculate lift coefficient (CL) for a NACA0012 airfoil at a given angle of attack (alpha)
double CalculateCL(int alpha) {
    // Hardcoded lift coefficient (CL) data for NACA0012 airfoil
    // Values based on typical lift curve slope and experimental data
    double cl[16] = { 0.000,0.120,0.240,0.360,0.480,0.599,0.719,0.838,0.957,1.075,1.193,1.311,1.428,1.545,1.662,1.777 };
    return cl[alpha];
}

// Function to calculate drag coefficient (CD) for a NACA0012 airfoil at a given angle of attack (alpha) and Reynolds number (Re)
double CalculateCD(int alpha, int Re) {
    // Hardcoded drag coefficient (CD) data for NACA0012 airfoil
    // Values based on experimental data and typical drag characteristics
    double Cd[3][16] = { {0.02403,0.02421,0.02476,0.02564,0.02691,0.02891,0.03163,0.02564,0.03103,0.03314,0.03192,0.03486,0.04089,0.04707,0.05541,0.06841},
        {0.01821,0.01832,0.01871,0.01932,0.02019,0.02126,0.02267,0.02437,0.02659,0.02218,0.02624,0.02869,0.03172,0.0347,0.03901,0.04377},
        {0.01707,0.0172,0.01756,0.01807,0.01873,0.01971,0.02083,0.02228,0.02401,0.02331,0.0238,0.02601,0.02881,0.0322,0.03607,0.04034}
    };
    return Cd[Re][alpha];
}

AirfoilFrame::AirfoilFrame( wxWindow* parent )
	: AirfoilFrameGUI( parent )
{
    //! initialization
    /*
    m_propertyGridItem_medium->SetChoices(getMediumList());
    m_propertyGridItem_medium->SetValue(0);
    m_propertyGridItem_geometry->SetChoices(getGeometryList());
    m_propertyGridItem_geometry->SetValue(0);
    m_propertyGridItem_flow->SetChoices(getFlowList());
    m_propertyGridItem_flow->SetValue(0);
    m_propertyGridItem_smoke->SetValue(true);
    m_propertyGridItem_density->SetValue(1000.0);
    m_propertyGridItem_overrelaxation->SetValue(1.9);
    m_propertyGridItem_resolution->SetValue(50);
    m_propertyGridItem_nb_cpu->SetValue(4);
    */
    //    m_panel_scene->SetBackgroundColour( *wxWHITE);
    int initialWidth = 1024;
    int initialHeight = 500;

    region = make_shared<Region>(initialHeight, initialWidth, 1.0);
    region->setupRegion(-1, 1.9);
    canvas = new Canvas(m_panel_scene, region);

    wxBoxSizer *bSizer;
    bSizer = new wxBoxSizer(wxVERTICAL);
    bSizer->Add(canvas, 1, wxEXPAND, 5);
    m_panel_scene->SetSizer(bSizer);
    m_panel_scene->SetAutoLayout(true);

    samplePlot1();
    wxBoxSizer* plotSizer1 = new wxBoxSizer(wxVERTICAL);
    plotSizer1->Add(m_plot1, 1, wxEXPAND, 5);
    m_panel_plot1->SetSizer(plotSizer1);
    m_panel_plot1->SetAutoLayout(true);

    samplePlot2();
    wxBoxSizer* plotSizer2 = new wxBoxSizer(wxVERTICAL);
    plotSizer2->Add(m_plot2, 1, wxEXPAND, 5);
    m_panel_plot2->SetSizer(plotSizer2);
    m_panel_plot2->SetAutoLayout(true);

    samplePlot3();
    wxBoxSizer* plotSizer3 = new wxBoxSizer(wxVERTICAL);
    plotSizer3->Add(m_plot3, 1, wxEXPAND, 5);
    m_panel_plot3->SetSizer(plotSizer3);
    m_panel_plot3->SetAutoLayout(true);

    SetAutoLayout(TRUE);

    m_notebook2->SetSelection(0);

    m_timer = new wxTimer(this);
    this->Connect(wxEVT_TIMER, wxTimerEventHandler(Canvas::animate), NULL, this);

    compute = true;
    SetSize(initialWidth, initialHeight);
    Center();

}

AirfoilFrame::~AirfoilFrame()
{
    delete m_timer;
    delete canvas;
}

void AirfoilFrame::simulate()
{
    if (!canvas->region->paused)
        canvas->region->fluid->simulate(canvas->region->dt, canvas->region->gravity, canvas->region->numIters);
    canvas->region->frameNr++;
}

void AirfoilFrame::update()
{
    simulate();
}

void AirfoilFrame::samplePlot1()
{
    // Create a wxMathPlot control
    m_plot1 = new mpWindow(m_panel_plot1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);

    wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    // Set plot titles and labels
    mpScaleX* xaxis = new mpScaleX(wxT("Angle of Attack (Alpha)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    //mpScaleX* xaxis = new mpScaleX(wxT("Drag Coefficient (CD)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Lift Coefficient (CL)"), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);

    m_plot1->SetMargins(30, 30, 30, 30);
    m_plot1->AddLayer(xaxis);
    m_plot1->AddLayer(yaxis);
    // Generate data points for plotting
    vector<XFoilPolarParser> polarTables = {
        XFoilPolarParser("data/xf-n0012-il-50000.txt"),
        XFoilPolarParser("data/xf-n0012-il-100000.txt"),
        XFoilPolarParser("data/xf-n0012-il-200000.txt"),
        XFoilPolarParser("data/xf-n0012-il-500000.txt"),
        XFoilPolarParser("data/xf-n0012-il-1000000.txt")
    };
    vector<double> alpha, lift;
    const wxColour penlist[5] = {*wxBLUE, *wxCYAN, *wxGREEN, *wxRED, *wxLIGHT_GREY};
    for (auto &i : polarTables) {
        alpha = i.getAlpha();
        lift = i.getCL();
        // Create a mpFXYVector layer
        mpFXYVector* vectorLayer = new mpFXYVector(_("Re"));
        vectorLayer->SetData(alpha, lift);
        vectorLayer->SetContinuity(true);
        wxPen vectorpen(penlist[&i - &polarTables[0]], 2, wxSOLID);
        vectorLayer->SetPen(vectorpen);
        vectorLayer->SetDrawOutsideMargins(false);
        wxString strN;
        strN.Printf("%d", i.getReynolds());
        vectorLayer->SetName(strN);
        m_plot1->AddLayer(vectorLayer);
    }
    m_plot1->AddLayer(new mpText(wxT("NACA0012 Airfoil Angle of Attack vs. Lift"), 10, 10));
    wxBrush hatch(wxColour(200, 200, 200), wxSOLID);
    m_plot1->AddLayer(nfo = new mpInfoCoords(wxRect(80, 20, 10, 10), wxTRANSPARENT_BRUSH)); //&hatch));
    nfo->SetVisible(false);
    wxBrush hatch2(wxColour(163, 208, 212), wxSOLID);
    mpInfoLegend* leg;
    m_plot1->AddLayer(leg = new mpInfoLegend(wxRect(200, 20, 40, 40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetItemMode(mpLEGEND_SQUARE);
    leg->SetVisible(true);
    
    vector<pair<double,double>> pairs;
    for (auto &i : polarTables) {
        pairs.push_back(i.getMinMaxAlpha());
    }
    auto p = std::minmax_element(pairs.begin(), pairs.end());
    auto minAlpha = p.first->first;
    auto maxAlpha = p.second->second;
    
    pairs.clear();
    for (auto &i : polarTables) {
        pairs.push_back(i.getMinMaxCL());
    }
    p = std::minmax_element(pairs.begin(), pairs.end());
    auto minCL = p.first->first;
    auto maxCL = p.second->second;
    
    m_plot1->Fit(minAlpha, maxAlpha, minCL, maxCL);
}


void AirfoilFrame::samplePlot2()
{
    // Create a wxMathPlot control
    m_plot2 = new mpWindow(m_panel_plot2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);

    wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    // Set plot titles and labels
    mpScaleX* xaxis = new mpScaleX(wxT("Angle of Attack (Alpha)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Drag Coefficient (CD)"), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);

    m_plot2->SetMargins(30, 30, 30, 30);
    m_plot2->AddLayer(xaxis);
    m_plot2->AddLayer(yaxis);
    // Generate data points for plotting
    vector<XFoilPolarParser> polarTables = {
        XFoilPolarParser("data/xf-n0012-il-50000.txt"),
        XFoilPolarParser("data/xf-n0012-il-100000.txt"),
        XFoilPolarParser("data/xf-n0012-il-200000.txt"),
        XFoilPolarParser("data/xf-n0012-il-500000.txt"),
        XFoilPolarParser("data/xf-n0012-il-1000000.txt")
    };
    std::vector<double> alpha, drag;
    const wxColour penlist[5] = { *wxBLUE, *wxCYAN, *wxGREEN, *wxRED, *wxLIGHT_GREY };
    for (auto &i : polarTables) {
        alpha = i.getAlpha();
        drag = i.getCD();
        // Create a mpFXYVector layer
        mpFXYVector* vectorLayer = new mpFXYVector(_("Re"));
        vectorLayer->SetData(alpha, drag);
        vectorLayer->SetContinuity(true);
        wxPen vectorpen(penlist[&i - &polarTables[0]], 2, wxSOLID);
        vectorLayer->SetPen(vectorpen);
        vectorLayer->SetDrawOutsideMargins(false);
        wxString strN;
        strN.Printf("%d", i.getReynolds());
        vectorLayer->SetName(strN);
        m_plot2->AddLayer(vectorLayer);
    }
    m_plot2->AddLayer(new mpText(wxT("NACA0012 Airfoil Angle of Attack vs. Drag"), 10, 10));
    wxBrush hatch(wxColour(200, 200, 200), wxSOLID);
    m_plot2->AddLayer(nfo = new mpInfoCoords(wxRect(80, 20, 10, 10), wxTRANSPARENT_BRUSH)); //&hatch));
    nfo->SetVisible(false);
    wxBrush hatch2(wxColour(163, 208, 212), wxSOLID);
    mpInfoLegend* leg;
    m_plot2->AddLayer(leg = new mpInfoLegend(wxRect(200, 20, 40, 40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetItemMode(mpLEGEND_SQUARE);
    leg->SetVisible(true);
    
    vector<pair<double,double>> pairs;
    for (auto &i : polarTables) {
        pairs.push_back(i.getMinMaxAlpha());
    }
    auto p = std::minmax_element(pairs.begin(), pairs.end());
    auto minAlpha = p.first->first;
    auto maxAlpha = p.second->second;
    
    pairs.clear();
    for (auto &i : polarTables) {
        pairs.push_back(i.getMinMaxCD());
    }
    p = std::minmax_element(pairs.begin(), pairs.end());
    auto minCD = p.first->first;
    auto maxCD = p.second->second;
    
    m_plot2->Fit(minAlpha, maxAlpha, minCD, maxCD);
}


void AirfoilFrame::samplePlot3()
{
    // Create a wxMathPlot control
    m_plot3 = new mpWindow(m_panel_plot3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);

    wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    // Set plot titles and labels
    mpScaleX* xaxis = new mpScaleX(wxT("Drag Coefficient (CD)"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Lift Coefficient (CL)"), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);

    m_plot3->SetMargins(30, 30, 30, 30);
    m_plot3->AddLayer(xaxis);
    m_plot3->AddLayer(yaxis);
    // Generate data points for plotting
    vector<XFoilPolarParser> polarTables = {
        XFoilPolarParser("data/xf-n0012-il-50000.txt"),
        XFoilPolarParser("data/xf-n0012-il-100000.txt"),
        XFoilPolarParser("data/xf-n0012-il-200000.txt"),
        XFoilPolarParser("data/xf-n0012-il-500000.txt"),
        XFoilPolarParser("data/xf-n0012-il-1000000.txt")
    };
    std::vector<double> lift, drag;
    const wxColour penlist[5] = { *wxBLUE, *wxCYAN, *wxGREEN, *wxRED, *wxLIGHT_GREY };
    for (auto &i : polarTables) {
        lift = i.getCL();
        drag = i.getCD();
        // Create a mpFXYVector layer
        mpFXYVector* vectorLayer = new mpFXYVector(_("Re"));
        vectorLayer->SetData(drag, lift);
        vectorLayer->SetContinuity(true);
        wxPen vectorpen(penlist[&i - &polarTables[0]], 2, wxSOLID);
        vectorLayer->SetPen(vectorpen);
        vectorLayer->SetDrawOutsideMargins(false);
        wxString strN;
        strN.Printf("%d", i.getReynolds());
        vectorLayer->SetName(strN);
        m_plot3->AddLayer(vectorLayer);
    }
    m_plot3->AddLayer(new mpText(wxT("NACA0012 Airfoil Drag vs. Lift"), 10, 10));
    wxBrush hatch(wxColour(200, 200, 200), wxSOLID);
    m_plot3->AddLayer(nfo = new mpInfoCoords(wxRect(80, 20, 10, 10), wxTRANSPARENT_BRUSH)); //&hatch));
    nfo->SetVisible(false);
    wxBrush hatch2(wxColour(163, 208, 212), wxSOLID);
    mpInfoLegend* leg;
    m_plot3->AddLayer(leg = new mpInfoLegend(wxRect(200, 20, 40, 40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetItemMode(mpLEGEND_SQUARE);
    leg->SetVisible(true);
    
    vector<pair<double,double>> pairs;
    for (auto &i : polarTables) {
        pairs.push_back(i.getMinMaxCD());
    }
    auto p = std::minmax_element(pairs.begin(), pairs.end());
    auto minCD = p.first->first;
    auto maxCD = p.second->second;
    
    pairs.clear();
    for (auto &i : polarTables) {
        pairs.push_back(i.getMinMaxCL());
    }
    p = std::minmax_element(pairs.begin(), pairs.end());
    auto minCL = p.first->first;
    auto maxCL = p.second->second;
    
    m_plot3->Fit(minCD, maxCD, minCL, maxCL);
}


void AirfoilFrame::OnClose(wxCloseEvent &event)
{
    if (m_timer->IsRunning())
        m_timer->Stop();
    Destroy();
}

void AirfoilFrame::onStartButtonClick(wxCommandEvent &event)
{
    int regionNr = m_choice_medium->GetSelection();//m_propertyGridItem_medium->GetValue().GetLong();
    setDafaultsForMedium(regionNr);
    wxAny value;

    //value = m_propertyGridItem_geometry->GetValue();
    canvas->region->geometry = indexToOBJ(m_choice_geometry->GetSelection());

    //value = m_propertyGridItem_streamlines->GetValue();
    canvas->region->showStreamlines = m_checkBox_streamlines->GetValue();//value.As<bool>();

    //value = m_propertyGridItem_vel_vec->GetValue();
    canvas->region->showVelocityVectors = m_checkBox_velocities->GetValue();// value.As<bool>();

    canvas->region->showPressure = m_checkBox_pressure->GetValue();// value.As<bool>();

    //value = m_propertyGridItem_smoke->GetValue();
    canvas->region->showSmoke = m_checkBox_smoke->GetValue(); // value.As<bool>();

    //value = m_propertyGridItem_overrelaxation->GetValue();
    canvas->region->overRelaxation = m_checkBox_overRelax->GetValue() == true ? 1.9 : 1.0; // value.As<double>();

    //value = m_propertyGridItem_resolution->GetValue();
    //canvas->region->resolution = value.As<int>();

    //value = m_propertyGridItem_nb_cpu->GetValue();
    //canvas->region->fluid->numThreads = value.As<int>();

    //value = m_propertyGridItem_density->GetValue();
    //canvas->region->fluid->density = value.As<double>();

    // canvas->region->updateRegionSize(canvas->height(),canvas->width());

    if (compute)
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_("running..."), 1);
#endif
        if (m_timer->IsRunning())
            m_timer->Stop();
        m_timer->Start(1);
        canvas->region->setupRegion(regionNr, canvas->region->overRelaxation, canvas->region->resolution, canvas->region->fluid->density, canvas->region->fluid->numThreads);
        /*
        m_propertyGridItem_streamlines->SetValue(canvas->region->showStreamlines);
        m_propertyGridItem_vel_vec->SetValue(canvas->region->showVelocityVectors);
        m_propertyGridItem_smoke->SetValue(canvas->region->showSmoke);
        m_propertyGridItem_geoLoc->SetValue(canvas->region->showGeometryPosition);
        m_propertyGridItem_nb_cpu->SetValue(canvas->region->fluid->numThreads);
        */
        compute = false;
        canvas->region->paused = false;
        m_button_start->SetLabel("Stop");
        m_checkBox_pause->SetValue(false);
        update();
        // setToolTip(ui->comboBox->currentText());
    }
    else
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_(""), 1);
#endif
        // timer->stop();
        m_timer->Stop();
        compute = true;
        m_button_start->SetLabel("Start");
        m_checkBox_pause->SetValue(false);
        canvas->region->paused = true;
        update();
    }
    //    update();
}

void AirfoilFrame::onCheckBoxChecked(wxCommandEvent &event)
{
    canvas->region->paused = event.IsChecked();
    if (canvas->region->paused)
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_("paused..."), 1);
#endif
    }
    else
    {
#if wxUSE_STATUSBAR
        statusBar->SetStatusText(_("running..."), 1);
#endif
    }
}

void AirfoilFrame::OnGeometryChanged(wxCommandEvent& event)
{
    canvas->region->geometry = indexToOBJ(event.GetSelection());
    update();
}
void AirfoilFrame::OnStreamlinesChecked(wxCommandEvent& event)
{
    canvas->region->showStreamlines = event.IsChecked();
    update();
}
void AirfoilFrame::OnVelocitiesChecked(wxCommandEvent& event)
{
    canvas->region->showVelocityVectors = event.IsChecked();
    update();
}
void AirfoilFrame::OnPressureChecked(wxCommandEvent& event)
{
    canvas->region->showPressure = event.IsChecked();
    update();
}
void AirfoilFrame::OnSmokeChecked(wxCommandEvent& event)
{
    canvas->region->showSmoke = event.IsChecked();
    update();
}
void AirfoilFrame::OnOverRelaxationChecked(wxCommandEvent& event)
{
    canvas->region->overRelaxation = event.IsChecked() == true ? 1.9 : 1.0;
    update();
}
void AirfoilFrame::OnMediumChanged(wxCommandEvent& event)
{
    canvas->region->RegionNr = event.GetSelection();
    setDafaultsForMedium(canvas->region->RegionNr);
    update();
}

void AirfoilFrame::setDafaultsForMedium(int _regionNr)
{
    switch (_regionNr)
    {
    case 0:
    case 1:
        m_checkBox_streamlines->SetValue(false);
        m_checkBox_velocities->SetValue(false);
        m_checkBox_pressure->SetValue(false);
        m_checkBox_smoke->SetValue(true);

        break;
    case 2:
        m_checkBox_streamlines->SetValue(false);
        m_checkBox_velocities->SetValue(false);
        m_checkBox_pressure->SetValue(true);
        m_checkBox_smoke->SetValue(false);
        break;
    case 3:
        m_checkBox_streamlines->SetValue(false);
        m_checkBox_velocities->SetValue(false);
        m_checkBox_pressure->SetValue(false);
        m_checkBox_smoke->SetValue(true);
        m_checkBox_overRelax->SetValue(false);
        break;
    default:
        break;
    }
}
