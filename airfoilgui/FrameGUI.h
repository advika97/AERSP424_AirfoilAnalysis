///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AirfoilFrameGUI
///////////////////////////////////////////////////////////////////////////////
class AirfoilFrameGUI : public wxFrame
{
	private:

	protected:
		wxStatusBar* statusBar;
		wxNotebook* m_notebook2;
		wxPanel* m_panel_properties;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel7;
		wxChoice* m_choice_medium;
		wxChoice* m_choice_geometry;
		wxCheckBox* m_checkBox_streamlines;
		wxCheckBox* m_checkBox_velocities;
		wxCheckBox* m_checkBox_pressure;
		wxCheckBox* m_checkBox_smoke;
		wxCheckBox* m_checkBox_overRelax;
		wxButton* m_button_start;
		wxCheckBox* m_checkBox_pause;
		wxPanel* m_panel_scene;
		wxPanel* m_panel8;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_panel9;
		wxPropertyGridManager* m_propertyGridManager2;
		wxPropertyGridPage* m_propertyGridPage2;
		wxPGProperty* m_propertyGridItem3;
		wxPGProperty* m_propertyGridItem4;
		wxPGProperty* m_propertyGridItem5;
		wxPGProperty* m_propertyGridItem6;
		wxPropertyGridManager* m_propertyGridManager21;
		wxPropertyGridPage* m_propertyGridPage3;
		wxPGProperty* m_propertyGridItem10;
		wxPGProperty* m_propertyGridItem8;
		wxPGProperty* m_propertyGridItem9;
		wxPropertyGridManager* m_propertyGridManager3;
		wxPropertyGridPage* m_propertyGridPage4;
		wxPGProperty* m_propertyGridItem111;
		wxPGProperty* m_propertyGridItem11;
		wxPGProperty* m_propertyGridItem12;
		wxPGProperty* m_propertyGridItem13;
		wxPanel* m_panel_plot;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMediumChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGeometryChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStreamlinesChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnVelocitiesChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPressureChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSmokeChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOverRelaxationChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onStartButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCheckBoxChecked( wxCommandEvent& event ) { event.Skip(); }


	public:

		AirfoilFrameGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Airfoil"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1095,534 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AirfoilFrameGUI();

		void m_splitter3OnIdle( wxIdleEvent& )
		{
			m_splitter3->SetSashPosition( 0 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( AirfoilFrameGUI::m_splitter3OnIdle ), NULL, this );
		}

		void m_splitter4OnIdle( wxIdleEvent& )
		{
			m_splitter4->SetSashPosition( 0 );
			m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( AirfoilFrameGUI::m_splitter4OnIdle ), NULL, this );
		}

};

