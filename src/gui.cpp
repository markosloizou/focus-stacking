#include "gui.h"
#include "GLcanvas.h"


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(MY_FILE_SAVE_ID, MyFrame::SaveCanvas)
    EVT_MENU(FILE_OPEN_ID, MyFrame::OnFileOpen)
	EVT_TOOL(SAVE_TOOLBAR_ID, MyFrame::SaveCanvas)
	EVT_MOUSEWHEEL(MyFrame::OnMouse)
    EVT_SLIDER(MATRIX_SIZE_SLIDER_ID, MyFrame::OnMatrixSizeRelease)
	EVT_SLIDER(SV_SLIDER_ID, MyFrame::OnSVRelease)  
    EVT_SLIDER(OPACITY_SLIDER_ID, MyFrame::OnOpacityRelease)
    EVT_BUTTON(MY_RUN_BUTTON_ID, MyFrame::OnRunButton)
    EVT_CHOICE(METHOD_CHOICE_ID, MyFrame::OnChoice)
    EVT_SLIDER(LBP_THRESHOLD_SLIDER_ID, MyFrame::OnLBPThresholdSlider)
    EVT_SLIDER(LBP_RADIUS_SLIDER_ID, MyFrame::OnLBPRadiusSlider)
    EVT_SLIDER(LBP_REGION_RADIUS_SLIDER_ID, MyFrame::OnLBPRegionRadiusSlider)
    EVT_LISTBOX_DCLICK(FILE_LIST_BOX_ID, MyFrame::OnFileListDClick)
    EVT_BUTTON(PROCESS_IMAGES_BUTTON_ID, MyFrame::OnProcessImages)
    EVT_SPINCTRLDOUBLE(SHARPNESS_CONTROL_ID, MyFrame::OnMinSharpnessSlider)
END_EVENT_TABLE()


MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
    long style):wxFrame(parent, wxID_ANY, title, pos, size, style)
{
    opacity = (ushort)pow(2,16)/2;
    previousRunSize = 0;
    
    
   
    
    // Set up top menu bar
    wxMenuBar *menuBar = new wxMenuBar;
    wxMenu *fileMenu = new wxMenu; 
    wxMenu *viewMenu = new wxMenu;
    wxMenu *windowMenu = new wxMenu;
   /// THIS CREATES ERRORS -- WHY?!?!
    fileMenu->Append(FILE_OPEN_ID, ("&Open\tCtrl-O"));
    fileMenu->Append(MY_FILE_SAVE_ID,_("&Save As...\tCtrl-S"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("&Quit\tCtrl-Q"));

    viewMenu->AppendSeparator();
    windowMenu->AppendSeparator();

    
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(viewMenu, _("&View"));
    menuBar->Append(windowMenu, _("&Window"));
    SetMenuBar(menuBar);    

    
    // Set up canvas using GLCanvas. This is where the signals are drawn
    canvas = new MyGLCanvas(left_canvas_window, this ,wxID_ANY);
    canvas_sizer->Add(canvas, 1, wxEXPAND);
    left_canvas_window->SetSizer(canvas_sizer);
    
    //  ****           Method Selector      ***     ///
    
    wxArrayString choices;
    choices.Add("SVD");
    choices.Add("LBP");
    //create choice selectror
    method_selection_choice = new wxChoice(right_button_window, METHOD_CHOICE_ID, wxDefaultPosition, wxDefaultSize, choices);
    method_selection_choice->SetSelection(0);
    current_method = svd_m;
    //add it to the sizer
    right_button_sizer->Add(method_selection_choice,0, wxTOP|wxEXPAND|wxALL, 5); //set position
    
    

    /// ***     SVD     ****    ///
    
    //MATRIX size
    svd_controls_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, _("Span from pixel")), 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    svd_controls_sizer->Add(matrix_size_value, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    matrix_size_slider = new wxSlider(right_button_window, MATRIX_SIZE_SLIDER_ID, 10, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);	// The slider position will range from 0 to 100 and start at 50
    
    svd_controls_sizer->Add(matrix_size_slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5);
    matrix_size_slider -> SetValue(3);
    MatrixSize = 3;
    
    
    // Singular Values number
    svd_controls_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, _("Singular Values")), 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    svd_controls_sizer->Add(singular_values_size, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    singular_values_slider = new wxSlider(right_button_window, SV_SLIDER_ID, 2, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);	// The slider position will range from 0 to 100 and start at 50
    svd_controls_sizer->Add(singular_values_slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5);
    singular_values_slider->SetValue(1);
    SV = 1;
    
    
    //add svd sizer to right_button_sizer
    right_button_sizer->Add(svd_controls_sizer,0,wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL);
    

    ////    ****    LBP     ****    /////
    
    lbp_threshold = 0.5;
    lbp_radius = 1.0;
    lbp_region_radius = 5;
    
    
    // threshold
    
    lbp_controls_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, _("Threshold")), 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    lbp_controls_sizer->Add(lbp_threshold_value, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    lbp_threshold_slider = new wxSlider(right_button_window, LBP_THRESHOLD_SLIDER_ID, 4, 1, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    lbp_controls_sizer->Add(lbp_threshold_slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5);
    
    // radius
    
    lbp_controls_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, wxT("Radius")), 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    lbp_controls_sizer->Add(lbp_radius_value, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT,5);
    lbp_radius_slider = new wxSlider(right_button_window, LBP_RADIUS_SLIDER_ID, 10, 0,190, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    lbp_controls_sizer->Add(lbp_radius_slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5);

    // region radius
    
    lbp_controls_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, wxT("Region Radius")), 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    lbp_controls_sizer->Add(lbp_region_radius_value, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT,5);
    lbp_region_radius_slider = new wxSlider(right_button_window, LBP_REGION_RADIUS_SLIDER_ID, 5, 0,200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    lbp_controls_sizer->Add(lbp_region_radius_slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5);
    
    
    //add to general sizer
    
    right_button_sizer->Add(lbp_controls_sizer,0,wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL);
    
    
    
    ////    ****    General Controls    *** /////

    
    //Opacity Controls
    right_button_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, _("Opacity")), 0, wxALIGN_CENTER_HORIZONTAL| wxTOP | wxLEFT | wxRIGHT, 5);
    right_button_sizer->Add(opacity_value, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxLEFT|wxRIGHT, 5);
    opacity_slider = new wxSlider(right_button_window, OPACITY_SLIDER_ID, 50, 0, 100,  wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    right_button_sizer->Add(opacity_slider, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5);
    
    // Minimum sharpness controls
    right_button_sizer->Add(new wxStaticText(right_button_window, wxID_ANY, wxT("Min Sharpess")), 0, wxALIGN_CENTER_HORIZONTAL| wxTOP | wxLEFT | wxRIGHT, 5);
   
    min_sharpness = new wxSpinCtrlDouble(right_button_window, SHARPNESS_CONTROL_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.00, 1.00,0.00, 0.001);
    right_button_sizer->Add(min_sharpness, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL,5);
    
    //ListBox
    names.Add("Test image");
    fileList = new wxListBox(right_button_window, FILE_LIST_BOX_ID, wxDefaultPosition, wxSize(-1,150), names, wxLB_SINGLE| wxLB_NEEDED_SB);
    right_button_sizer->Add(fileList, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxEXPAND|wxALL,5);
    fileList->SetSelection(0);
    
    

    //Run Button
    right_button_sizer->Add(new wxButton(right_button_window, MY_RUN_BUTTON_ID, _("Run")), 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    
    //Process Images 
    right_button_sizer->Add(new wxButton(right_button_window,PROCESS_IMAGES_BUTTON_ID, wxT("Merge")), 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    
    //Progress gauge
    progress_gauge = new wxGauge(right_button_window,wxID_ANY,100);
    right_button_sizer->Add(progress_gauge, 0, wxALIGN_CENTER_HORIZONTAL|wxALL,5);
    progress_gauge->Show(false);
    progress_gauge->SetRange(100);
    progress_gauge->SetValue(0);
    
    right_button_window->SetSizer(right_button_sizer);
    
    
    frame_sizer->Add(left_canvas_window, 1, wxEXPAND);
    frame_sizer->Add(right_button_window, 0, wxALIGN_TOP|wxALIGN_RIGHT|wxTOP|wxBOTTOM|wxRIGHT, 5);

    
    frame_with_status_sizer->Add(frame_sizer, 1, wxEXPAND);
    statusBar = new wxStatusBar(this,wxID_ANY, wxSTB_DEFAULT_STYLE, "sharpness");
    frame_with_status_sizer->Add(statusBar, 0, wxALIGN_BOTTOM);
    
    statusBar->SetStatusText("sharpness");
    //SetSizeHints(600, 400);
    
    SetSizer(frame_with_status_sizer);
    
    // hide
    
    lbp_controls_sizer->Show(false);  
    
}

void MyFrame::OnExit(wxCommandEvent& event)     			// Event handler for exit menu item
{

}
void MyFrame::ShowDialog(wxCommandEvent &event)				// Event handler to show the bottom dialog window
{

}

void MyFrame::OnMouse(wxMouseEvent &event)					// Event handler for zooming using the mouse wheel
{
    
}

void MyFrame::OnChoice(wxCommandEvent &event)
{
    int choice = method_selection_choice->GetSelection();
    std::cout << "choice: " << choice << std::endl;
    svd_controls_sizer->Show(false);
    lbp_controls_sizer->Show(false);
    switch(choice)
    {
        case 0:
            svd_controls_sizer->Show(true);
            current_method = svd_m;
            break;
        case 1:
            lbp_controls_sizer->Show(true);
            current_method = lbp_m;
            break;
        default:
            svd_controls_sizer->Show(true);
            current_method = svd_m;
            break;
    }
    right_button_sizer->Layout();
    frame_with_status_sizer->Layout();
}

void MyFrame::OnMatrixSizeRelease(wxCommandEvent &event)
{
     MatrixSize = matrix_size_slider->GetValue(); 

    std::string vert_zoom_str =  std::to_string(MatrixSize) ;
    wxString vert_zoom_wxstr(vert_zoom_str.c_str(), wxConvUTF8);

    matrix_size_value->SetLabel(vert_zoom_wxstr);		// Sets the value of the text string used to display the zoom value


}

void MyFrame::OnSVRelease(wxCommandEvent &event)
{
        int sv_slider = singular_values_slider->GetValue();
        float proportion = (float) sv_slider/100.0;
        
        SV = (int)round(proportion*MatrixSize);
        
        std::string SVs = std::to_string(SV);
        wxString SVwxstr(SVs.c_str(), wxConvUTF8);
        
        singular_values_size->SetLabel(SVwxstr);
}

void MyFrame::OnOpacityRelease(wxCommandEvent &event)
{
    int op = opacity_slider->GetValue();
    
    canvas -> SetOpacity(op);
    
    std::string str = std::to_string(op) + "%";
    wxString wx_str(str.c_str(), wxConvUTF8);
    opacity_value->SetLabel(wx_str);
}

void MyFrame::OnRunButton(wxCommandEvent &event)
{
    switch(current_method)
    {
        case svd_m:
            RunSVD();
            break;
        case lbp_m:
            RunLBP();
            break;
        default:
            break;
    }
}

void MyFrame::RunSVD()
{
    previousRunSize = MatrixSize;
    progress_gauge->Show(true);
    right_button_sizer->Layout();
    frame_with_status_sizer->Layout();
    right_button_sizer->Layout();
    canvas->Run_SVD(MatrixSize, SV);
    progress_gauge->Show(false);
}

void MyFrame::RunLBP()
{
    progress_gauge->Show(true);
    right_button_sizer->Layout();
    frame_with_status_sizer->Layout();
    canvas->Run_LBP(lbp_threshold, lbp_radius, lbp_region_radius);
    progress_gauge->Show(false);
}


void MyFrame::setGauge(int x)
{
    
    progress_gauge->SetValue(x);
    wxTheApp->Yield();
    progress_gauge->Refresh();
}

void MyFrame::OnLBPThresholdSlider(wxCommandEvent &event)
{
    int thresh = lbp_threshold_slider->GetValue();
   
    
    lbp_threshold = (float) thresh/1000.0;
    
    std::stringstream stream;
    stream << std::fixed << std::setprecision(3) << lbp_threshold;
    std::string str = stream.str();
    
    
    wxString wxstr(str.c_str(), wxConvUTF8);
    
    lbp_threshold_value->SetLabel(wxstr);
}

void MyFrame::OnLBPRadiusSlider(wxCommandEvent &event)
{
    int value = lbp_radius_slider->GetValue();
    std::string str;
    
    if(value < 100)
    {
        lbp_radius = (float)value/10;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << lbp_radius;
        str = stream.str();
    }
    else
    {
        lbp_radius = 10 + (float)value - 100;
        str = std::to_string((int)round(lbp_radius));
    }
    
    
    str = str + " px";
    wxString wxstr(str.c_str(), wxConvUTF8);
    
    lbp_radius_value->SetLabel(wxstr);
}

void MyFrame::OnLBPRegionRadiusSlider(wxCommandEvent &event)
{
    lbp_region_radius = lbp_region_radius_slider->GetValue();
    
    std::string str = std::to_string(lbp_region_radius);
    str = str + " px";
    wxString wxstr(str.c_str(), wxConvUTF8);
    
    lbp_region_radius_value->SetLabel(wxstr);
}

void MyFrame::setStatusBarText(std::string s)
{
    statusBar->SetStatusText(s);
}


void MyFrame::SaveCanvas(wxCommandEvent &event)				// Event handler for using the saving toolbar icon
{
    wxFileDialog saveFileDialog(this, "Save Image","","", "TIFF (*.tiff)|*.tiff",wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT);
    
    wxString save_file;
    
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        std::cout << "no selection" << std::endl;
        return;     // the user changed idea...
    }
    
    save_file = saveFileDialog.GetPath();
    
    std::string file = std::string(save_file.mb_str());
    
    canvas->save(file);
}


void MyFrame::OnFileOpen(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, "Open Images","","", "Raw Files (*.NEF)|*.NEF",wxFD_OPEN|wxFD_CHANGE_DIR|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        std::cout << "no selection" << std::endl;
        return;     // the user changed idea...
    }
   
    openFileDialog.GetFilenames(names);
    openFileDialog.GetPaths(paths); 
   
    if(filenames.size() != 0) filenames.clear();
    if(fullPaths.size() != 0) fullPaths.clear();
    for(int i = 0; i < paths.GetCount(); i++)
    {
        filenames.push_back(std::string(names[i].mb_str()) );
        fullPaths.push_back(std::string(paths[i].mb_str()) );
    } 
    
    for(int i = 0; i < fullPaths.size(); i++)
    {
        std::cout << fullPaths[i] << std::endl;
    }
        
    fileList->Deselect(fileList->GetSelection());
    fileList->Clear();
    fileList->InsertItems(names,0);
    fileList->SetSelection(0);
    canvas->SetCurrentImage(fullPaths[0]);
}


void MyFrame::OnFileListDClick(wxCommandEvent &event)
{
    std::string s = fullPaths[fileList->GetSelection()];
    canvas->SetCurrentImage(s);
}


void  MyFrame::OnProcessImages(wxCommandEvent &event)
{
    switch(current_method)
    {
        case svd_m:
            ProcessSVD();
            break;
        case lbp_m:
            ProcessLBP();
            break;
        default:
            break;
    }
}


void MyFrame::ProcessSVD()
{
    previousRunSize = MatrixSize;
    progress_gauge->Show(true);
    right_button_sizer->Layout();
    frame_with_status_sizer->Layout();
    right_button_sizer->Layout();
    canvas->Process_SVD(MatrixSize, SV, fullPaths);
    progress_gauge->Show(false);
}

void MyFrame::ProcessLBP()
{
    progress_gauge->Show(true);
    right_button_sizer->Layout();
    frame_with_status_sizer->Layout();
    canvas->Process_LBP(lbp_threshold, lbp_radius, lbp_region_radius, fullPaths);
    progress_gauge->Show(false);
}


void MyFrame::OnMinSharpnessSlider(wxSpinDoubleEvent& event)
{
    float value = min_sharpness->GetValue();
    //send value to canvas
    canvas->set_min_sharpness(value);
}
