#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/cshelp.h>
#include <wx/gauge.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>
#include <wx/listbox.h>
#include <wx/slider.h>
#include <wx/window.h>
#include <wx/spinctrl.h>

#include <iostream>
#include <string>
#include <math.h>

#include <iomanip> // setprecision
#include <sstream> // stringstream
// 
#include "data_types.h"


enum {
    MY_SPINCNTRL_ID = wxID_HIGHEST + 1,
    MY_TEXTCTRL_ID,
    MY_RUN_BUTTON_ID,
    MY_FILE_RUN_ID,
    MY_FILE_CONTINUE_ID,
    MY_FILE_SAVE_ID,
    FILE_OPEN_ID,
    TOOLBAR_ID,
    RUN_TOOLBAR_ID,
    CONTINUE_TOOLBAR_ID,
    START_TOOLBAR_ID,
    END_TOOLBAR_ID,
    MY_CONTINUE_BUTTON_ID,
    VERT_SLIDER_ID,
    HORZ_SLIDER_ID,
    SWITCH_LIST_ID,
    SWITCH_BUTTON_ID,
    SWITCH_LISTBOX_ID,
    MONITOR_LISTBOX_ID,
    SHOW_GRID_ID,
    SHOW_SETTINGS_ID,
    SHOW_DIALOG_ID,
    SAVE_TOOLBAR_ID,
    START_MENU_ID,
    END_MENU_ID,
    SAVE_MENU_ID,
    ZOOM_IN_MENU_ID,
    ZOOM_OUT_MENU_ID,
    HELP_MENU_ID,
    MATRIX_SIZE_SLIDER_ID,
    SV_SLIDER_ID,
    OPACITY_SLIDER_ID,
    METHOD_CHOICE_ID,
    LBP_THRESHOLD_SLIDER_ID,    
    LBP_RADIUS_SLIDER_ID,
    LBP_REGION_RADIUS_SLIDER_ID,
    FILE_LIST_BOX_ID,
    PROCESS_IMAGES_BUTTON_ID,
    SHARPNESS_CONTROL_ID,  
}; // widget identifiers

class MyGLCanvas;

class MyFrame: public wxFrame
{
    
    public:
        MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
            const wxSize& size, long style = wxDEFAULT_FRAME_STYLE); // constructor
        void setGauge(int x);
        void setStatusBarText(std::string s);
    private:
        method current_method;
        ushort opacity;
        int previousRunSize;

        void OnExit(wxCommandEvent& event);     			// Event handler for exit menu item
        void ShowDialog(wxCommandEvent &event);				// Event handler to show the bottom dialog window
        void SaveCanvas(wxCommandEvent &event);				// Event handler for using the saving toolbar icon
        void OnMouse(wxMouseEvent &event);					// Event handler for zooming using the mouse wheel
        void OnFileOpen(wxCommandEvent &event);
        void OnProcessImages(wxCommandEvent &event);
        void OnMinSharpnessSlider(wxSpinDoubleEvent& event);
        
        
        //// ***       General Controls ****    ////
        void OnRunButton(wxCommandEvent &event);
        void OnChoice(wxCommandEvent &event);
        void OnFileListDClick(wxCommandEvent &event);
        
        //// ****    SVD Controls ****  ////
        void OnMatrixSizeRelease(wxCommandEvent &event);
        void OnSVRelease(wxCommandEvent &event);
        void OnOpacityRelease(wxCommandEvent &event);
        
        void RunSVD();
        void ProcessSVD();
        
        ////    ****    LBP Controls    ****    ////
        void OnLBPThresholdSlider(wxCommandEvent &event);
        void OnLBPRadiusSlider(wxCommandEvent &event);
        void OnLBPRegionRadiusSlider(wxCommandEvent &event);
        
        void RunLBP();
        void ProcessLBP();
        
        MyGLCanvas *canvas;                     // OpenGL drawing area widget to draw traces

        //Windows for various items
        wxWindow *left_canvas_window = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(600,400), wxBORDER_SUNKEN, "Canvas");
        wxScrolledWindow* right_button_window = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(170,-1), wxVSCROLL, "Settings");		// -1 in size indicates that the default size should be used
        
        //combo box for method selection
        wxChoice *method_selection_choice;
        
        //sizers for components
        wxBoxSizer *canvas_sizer = new wxBoxSizer(wxVERTICAL);					// Sizer for the canvas
        
        wxBoxSizer *right_button_sizer = new wxBoxSizer(wxVERTICAL);            //sizer for controls
        wxBoxSizer *svd_controls_sizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *lbp_controls_sizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *frame_sizer = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer *frame_with_status_sizer = new wxBoxSizer(wxVERTICAL);
        
        
        // ****         SVD             **** ///
        
        // method sliders
        wxSlider *matrix_size_slider;		// Slider used for zooming vertically				
        wxSlider *singular_values_slider;		// Slider used for zooming horizontally
       
        
        //Text fields
        wxStaticText *matrix_size_value = new wxStaticText(right_button_window, wxID_ANY, "3");		// Used to display the current value of the vertical zoom
        wxStaticText *singular_values_size = new wxStaticText(right_button_window, wxID_ANY, "1");		// Used to display the current value of the horizontal zoom
        
        
        //variables
        int MatrixSize;
        int SV;
        
        
        // ****     LBP             ****    ///
        
        //lbp sliders

        wxSlider *lbp_threshold_slider;
        wxSlider *lbp_radius_slider;
        wxSlider *lbp_region_radius_slider;

        wxStaticText *lbp_threshold_value = new wxStaticText(right_button_window, wxID_ANY, "0.004");
        wxStaticText *lbp_radius_value = new wxStaticText(right_button_window, wxID_ANY, "1.0 px");
        wxStaticText *lbp_region_radius_value = new wxStaticText(right_button_window, wxID_ANY, "5 px");

        
        //variables
        float lbp_threshold;
        float lbp_radius;
        int lbp_region_radius;   
        
        
        //  ****    General     ****    //
        wxSlider *opacity_slider;
        
        wxSpinCtrlDouble *min_sharpness;
        
        wxStaticText *opacity_value =  new wxStaticText(right_button_window, wxID_ANY, "50%");		// Used to display the current value of the horizontal zoom

        
        //progress gauge
        wxGauge *progress_gauge;
        wxStatusBar *statusBar;
        
        
        //  ****    File Handling   ****    ////
        wxListBox *fileList;
        wxArrayString names;
        wxArrayString paths;
        std::vector<std::string> fullPaths;
        std::vector<std::string> filenames;
        
        
       
        

    DECLARE_EVENT_TABLE()
};
#endif
