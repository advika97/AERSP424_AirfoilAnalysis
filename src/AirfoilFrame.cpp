#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "AirfoilFrame.h"

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
