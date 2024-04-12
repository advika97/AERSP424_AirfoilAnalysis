///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "FrameGUI.h"

///////////////////////////////////////////////////////////////////////////

AirfoilFrameGUI::AirfoilFrameGUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	statusBar = this->CreateStatusBar( 2, wxSTB_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), wxSP_3D );
	m_panel_properties = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	m_splitter3 = new wxSplitterWindow( m_panel_properties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( AirfoilFrameGUI::m_splitter3OnIdle ), NULL, this );

	m_panel7 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxString m_choice_mediumChoices[] = { wxT("Wind Tunnel"), wxT("Wind Tunnel (High Res)"), wxT("Tank"), wxT("Paint") };
	int m_choice_mediumNChoices = sizeof( m_choice_mediumChoices ) / sizeof( wxString );
	m_choice_medium = new wxChoice( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_mediumNChoices, m_choice_mediumChoices, 0 );
	m_choice_medium->SetSelection( 0 );
	bSizer16->Add( m_choice_medium, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choice_geometryChoices[] = { wxT("Cylinder"), wxT("NACA 0012") };
	int m_choice_geometryNChoices = sizeof( m_choice_geometryChoices ) / sizeof( wxString );
	m_choice_geometry = new wxChoice( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_geometryNChoices, m_choice_geometryChoices, 0 );
	m_choice_geometry->SetSelection( 0 );
	bSizer16->Add( m_choice_geometry, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBox_streamlines = new wxCheckBox( m_panel7, wxID_ANY, wxT("Streamlines"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_checkBox_streamlines, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBox_velocities = new wxCheckBox( m_panel7, wxID_ANY, wxT("Velocities"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_checkBox_velocities, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBox_pressure = new wxCheckBox( m_panel7, wxID_ANY, wxT("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_checkBox_pressure, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBox_smoke = new wxCheckBox( m_panel7, wxID_ANY, wxT("Smoke"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_smoke->SetValue(true);
	bSizer16->Add( m_checkBox_smoke, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBox_overRelax = new wxCheckBox( m_panel7, wxID_ANY, wxT("Overrelaxation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_overRelax->SetValue(true);
	bSizer16->Add( m_checkBox_overRelax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_button_start = new wxButton( m_panel7, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button_start->SetMinSize( wxSize( -1,25 ) );

	bSizer16->Add( m_button_start, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBox_pause = new wxCheckBox( m_panel7, wxID_ANY, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_checkBox_pause, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( bSizer16, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );

	m_panel_scene = new wxPanel( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer141->Add( m_panel_scene, 1, wxEXPAND | wxALL, 5 );


	bSizer6->Add( bSizer141, 1, wxEXPAND, 5 );


	m_panel7->SetSizer( bSizer6 );
	m_panel7->Layout();
	bSizer6->Fit( m_panel7 );
	m_splitter3->Initialize( m_panel7 );
	bSizer14->Add( m_splitter3, 1, wxEXPAND, 5 );


	m_panel_properties->SetSizer( bSizer14 );
	m_panel_properties->Layout();
	bSizer14->Fit( m_panel_properties );
	m_notebook2->AddPage( m_panel_properties, wxT("Flowfield"), false );
	m_panel8 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );

	m_splitter4 = new wxSplitterWindow( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( AirfoilFrameGUI::m_splitter4OnIdle ), NULL, this );

	m_panel9 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );

	m_panel_plot1 = new wxPanel( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer19->Add( m_panel_plot1, 1, wxEXPAND | wxALL, 5 );

	m_panel_plot2 = new wxPanel( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer19->Add( m_panel_plot2, 1, wxEXPAND | wxALL, 5 );

	m_panel_plot3 = new wxPanel( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer19->Add( m_panel_plot3, 1, wxEXPAND | wxALL, 5 );


	m_panel9->SetSizer( bSizer19 );
	m_panel9->Layout();
	bSizer19->Fit( m_panel9 );
	m_splitter4->Initialize( m_panel9 );
	bSizer17->Add( m_splitter4, 1, wxEXPAND, 5 );


	m_panel8->SetSizer( bSizer17 );
	m_panel8->Layout();
	bSizer17->Fit( m_panel8 );
	m_notebook2->AddPage( m_panel8, wxT("Analysis"), true );

	bSizer9->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer9 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AirfoilFrameGUI::OnClose ) );
	m_choice_medium->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AirfoilFrameGUI::OnMediumChanged ), NULL, this );
	m_choice_geometry->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AirfoilFrameGUI::OnGeometryChanged ), NULL, this );
	m_checkBox_streamlines->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnStreamlinesChecked ), NULL, this );
	m_checkBox_velocities->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnVelocitiesChecked ), NULL, this );
	m_checkBox_pressure->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnPressureChecked ), NULL, this );
	m_checkBox_smoke->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnSmokeChecked ), NULL, this );
	m_checkBox_overRelax->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnOverRelaxationChecked ), NULL, this );
	m_button_start->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::onStartButtonClick ), NULL, this );
	m_checkBox_pause->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::onCheckBoxChecked ), NULL, this );
}

AirfoilFrameGUI::~AirfoilFrameGUI()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AirfoilFrameGUI::OnClose ) );
	m_choice_medium->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AirfoilFrameGUI::OnMediumChanged ), NULL, this );
	m_choice_geometry->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AirfoilFrameGUI::OnGeometryChanged ), NULL, this );
	m_checkBox_streamlines->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnStreamlinesChecked ), NULL, this );
	m_checkBox_velocities->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnVelocitiesChecked ), NULL, this );
	m_checkBox_pressure->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnPressureChecked ), NULL, this );
	m_checkBox_smoke->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnSmokeChecked ), NULL, this );
	m_checkBox_overRelax->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::OnOverRelaxationChecked ), NULL, this );
	m_button_start->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::onStartButtonClick ), NULL, this );
	m_checkBox_pause->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AirfoilFrameGUI::onCheckBoxChecked ), NULL, this );

}
