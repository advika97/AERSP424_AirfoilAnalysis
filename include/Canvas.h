#ifndef DRAW_H
#define DRAW_H

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/rawbmp.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <iostream>

#include "Region.h"

using namespace std;


class Canvas : public wxPanel
{
public:
    Canvas(wxWindow* parent, shared_ptr<Region> _region);
    ~Canvas() { delete[]m_pixelData; }
    shared_ptr<Region> region;
    void setRegion(shared_ptr<Region> s)
    {
        region=s;
    }

    bool mouseDown=false;


    void paint(wxDC& dc);
//private:
    void paintEvent(wxPaintEvent & event);

    void getSize(int* width,int *height);

    // some useful events

    void onMouseMoved(wxMouseEvent& event);
    void onMouseDown(wxMouseEvent& event);
    void onMouseReleased(wxMouseEvent& event);
    void animate(wxTimerEvent& event);

    //OBJ object=CYLINDER;

//     wxTimer *m_timer;
    int loop;
    wxBitmap m_Bitmap;
    unsigned char* m_pixelData;


    /* void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    wxDECLARE_EVENT_TABLE();
};

vector<int> getSciColor(double val, double minVal, double maxVal);


#endif // DRAW_H
